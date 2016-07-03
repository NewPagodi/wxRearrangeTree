/***************************************************************
 * Name:
 * Purpose:
 * Author:     ()
 * Created:   2016-07-03
 * Copyright:  ()
 * License:   wxWindows licence
 **************************************************************/

#include <stack>
#include <queue>
#include <wx/rearrangetree/wxrearrangetree.h>

class rtdbTreeData:public wxTreeItemData
{
    public:
        rtdbTreeData(int i,int j):m_itemId(i),m_auxId(j){}
        int GetItemId(){return m_itemId;}
        int GetAuxId(){return m_auxId;}
    private:
        int m_itemId;
        int m_auxId;
};

wxRearrangeTree::wxRearrangeTree()
{
}

wxRearrangeTree::wxRearrangeTree(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
{
    Create(parent, id, pos, size, style,name);
}

wxRearrangeTree::~wxRearrangeTree()
{
    m_bumpOrdStmt.Finalize();
    m_decOrdStmt.Finalize();
    m_newDBItemStmt.Finalize();
    m_updateOrderingStmt.Finalize();
    m_updateParentAndOrderingStmt.Finalize();
    m_deleteItemStmt.Finalize();
    m_addExpandedStmt.Finalize();
    m_remExpandedStmt.Finalize();
}

bool wxRearrangeTree::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
{
    if(!wxTreeCtrl::Create(parent, id, pos, size, style,wxDefaultValidator,name))
    {
        return false;
    }

    Init();
    return true;
}

void wxRearrangeTree::Init()
{
    this->Bind( wxEVT_KEY_UP, &wxRearrangeTree::OnTreeKeyUp, this );
    this->Bind( wxEVT_TREE_END_LABEL_EDIT,  &wxRearrangeTree::OnTreeEndLabelEdit, this );
    this->Bind( wxEVT_TREE_ITEM_EXPANDING, &wxRearrangeTree::OnTreeItemExpanding, this );
    this->Bind( wxEVT_TREE_ITEM_COLLAPSED, &wxRearrangeTree::OnTreeItemCollapsed, this );
    this->Bind( wxEVT_TREE_ITEM_EXPANDED, &wxRearrangeTree::OnTreeItemExpanded, this );
}

bool wxRearrangeTree::SetHelperAndData(wxRearrangeTreeHelper* m, wxSQLite3Database* dbp, const wxString& s, const wxString& e )
{
    m_treeHelper=m;
    m_db=dbp;
    m_tableName=s;
    m_expandTableName=e;
    m_fullVirtual=false;

    try
    {
        m_mapChildrenString=wxString::Format(
            "SELECT SubQ.ItemID,SubQ.AuxId,(SELECT count(*) FROM %s WHERE Parent=SubQ.ItemID)"
            "FROM "
            "(SELECT ItemID,Ordering,AuxId FROM %s WHERE Parent = ? ORDER BY Ordering) AS SubQ;"
            ,m_tableName,m_tableName);

        m_getChildrenString=wxString::Format(
            "SELECT ItemID FROM %s WHERE Parent=?;"
            ,m_tableName);

        m_getOrderingString=wxString::Format(
            "SELECT Ordering FROM %s WHERE ItemID=?;"
            ,m_tableName);

        m_getParentString=wxString::Format(
            "SELECT Parent FROM %s WHERE ItemID=?;"
            ,m_tableName);

        m_getNextOrdString=wxString::Format(
            "SELECT ifnull(max(Ordering)+1,1) FROM %s WHERE Parent = ?;"
            ,m_tableName);

        wxString getRootDataString=wxString::Format(
            "SELECT ItemID,AuxId FROM %s WHERE Parent=0;"
            ,m_tableName);

        wxString st=wxString::Format(
            "UPDATE %s SET Ordering=Ordering+1 WHERE Ordering>? AND Parent=?;"
            ,m_tableName);
        m_bumpOrdStmt=m_db->PrepareStatement(st);

        st=wxString::Format(
            "UPDATE %s SET Ordering=Ordering-1 WHERE Ordering>=? AND Parent=?;"
            ,m_tableName);
        m_decOrdStmt=m_db->PrepareStatement(st);

        st=wxString::Format(
            "INSERT INTO %s(ItemID,Parent,Ordering,AuxId) VALUES (null,?,?,?);"
            ,m_tableName);
        m_newDBItemStmt=m_db->PrepareStatement(st);

        st=wxString::Format(
            "UPDATE %s SET Ordering=? WHERE ItemID=?;"
            ,m_tableName);
        m_updateOrderingStmt=m_db->PrepareStatement(st);

        st=wxString::Format(
            "UPDATE %s SET Parent=?,Ordering=? WHERE ItemID=?;"
            ,m_tableName);
        m_updateParentAndOrderingStmt=m_db->PrepareStatement(st);

        st=wxString::Format(
            "DELETE FROM %s WHERE ItemID=?;"
            ,m_tableName);
        m_deleteItemStmt=m_db->PrepareStatement(st);

        if(!m_expandTableName.IsEmpty())
        {
            wxString m_getExpandedItemsString=wxString::Format(
                "SELECT ItemID FROM %s;"
                ,m_expandTableName);;

            st=wxString::Format(
                "INSERT INTO %s(ItemID) VALUES (?);"
                ,m_expandTableName);
            m_addExpandedStmt=m_db->PrepareStatement(st);

            st=wxString::Format(
                "DELETE FROM %s WHERE ItemID=?;"
                ,m_expandTableName);
            m_remExpandedStmt=m_db->PrepareStatement(st);

            st=wxString::Format(
                "DELETE FROM %s;"
                ,m_expandTableName);
            m_deleteAllExpandedStmt=m_db->PrepareStatement(st);

            st=wxString::Format(
                "DELETE FROM %s;"
                ,m_tableName);
            m_deleteAllItemsStmt=m_db->PrepareStatement(st);

            wxSQLite3Statement stmt = m_db->PrepareStatement(m_getExpandedItemsString);
            wxSQLite3ResultSet result = stmt.ExecuteQuery();

            while( result.NextRow() )
            {
                m_expandedItems.insert(result.GetInt(0));
            }
            result.Finalize();
            stmt.Finalize();
        }

        wxSQLite3Statement stmt = m_db->PrepareStatement(getRootDataString);
        wxSQLite3ResultSet result = stmt.ExecuteQuery();

        if( result.NextRow() )
        {
            wxTreeItemId root = this->AddRoot("");
            ConfigureNode(root,result.GetInt(0),result.GetInt(0));

            RestoreChildren(root);
        }
        result.Finalize();
        stmt.Finalize();
    }
    catch(wxSQLite3Exception& WXUNUSED(e))
    {
        //we should probably assert or notify of failure is some other way
        return false;
    }

    return true;
}

void wxRearrangeTree::SetFullVitual(bool b)
{
    m_fullVirtual=b;
}

bool wxRearrangeTree::GetFullVirtual()
{
    return m_fullVirtual;
}

int wxRearrangeTree::GetDbAuxId(const wxTreeItemId& item)
{
    if(rtdbTreeData* d = dynamic_cast<rtdbTreeData*>( this->GetItemData(item) ))
    {
        return d->GetAuxId();
    }
    else
    {
        return -1;
    }
}

int wxRearrangeTree::GetDbItemId(const wxTreeItemId& item)
{
    if(rtdbTreeData* d = dynamic_cast<rtdbTreeData*>( this->GetItemData(item) ))
    {
        return d->GetItemId();
    }
    else
    {
        return -1;
    }
}

wxTreeItemId wxRearrangeTree::NewRoot(int d)
{
    if(this->GetRootItem().IsOk())
    {
        return wxTreeItemId();
    }

    //If we're adding a new root, any data in the tables must be useless
    //so delete it:
    m_db->Begin();
    DumpAllData();

    //Add a new root item in the db
    int i=NewDbItem(0,1,d);
    m_db->Commit();

    //Add to the control
    wxTreeItemId root = this->AddRoot("");
    ConfigureNode(root,i,d);

    return root;
}

wxTreeItemId wxRearrangeTree::NewNextSibling(const wxTreeItemId &item, int Data)
{
    wxTreeItemId newitem;

    if(!item.IsOk())
    {
        return newitem ;
    }

    wxTreeItemId par=this->GetItemParent(item);
    if(!par.IsOk())
    {
        return newitem ;
    }

    int itemid=GetDbItemId(item);
    int Ordering=GetOrdering(itemid);
    int Parent=GetParent(itemid);

    BumpOrderingsAfter(Ordering, Parent);

    int newid=NewDbItem(Parent, Ordering+1, Data);
    newitem = this->InsertItem(par, item, wxEmptyString);
    ConfigureNode(newitem,newid,Data);

    return newitem;
}

wxTreeItemId wxRearrangeTree::NewFirstChild(const wxTreeItemId &item, int Data)
{
    wxTreeItemId newitem;

    if(!item.IsOk())
    {
        return newitem;
    }

    int thisID=GetDbItemId(item);
    BumpOrderingsAfter(0, thisID);
    int newid = NewDbItem(thisID,1,Data);

    newitem=this->PrependItem(item, wxEmptyString);
    ConfigureNode(newitem,newid,Data);

    //if the item previously had children, but was made empty by a move or
    //delete call, the item won't get a toggle button unless we do this:
    this->SetItemHasChildren(item,true);

    return newitem;
}

wxTreeItemId wxRearrangeTree::NewLastChild(const wxTreeItemId &item, int Data)
{
    wxTreeItemId newitem;

    if(!item.IsOk())
    {
        return newitem;
    }

    int thisID=GetDbItemId(item);
    int neword=GetNextOrderingIn(thisID);
    int newid = NewDbItem(thisID,neword,Data);

    newitem=this->AppendItem( item, wxEmptyString);
    ConfigureNode(newitem,newid,Data);

    //if the item previously had children, but was made empty by a move or
    //delete call, the item won't get a toggle button unless we do this:
    this->SetItemHasChildren(item,true);

    return newitem;
}

wxTreeItemId wxRearrangeTree::MoveAfter(const wxTreeItemId& src,const wxTreeItemId& dest)
{
    //step 1: make sure all the necessary nodes are ok
    if(!MoveChecker(src,dest))
    {
        wxBell();
        return wxTreeItemId();
    }

    wxTreeItemId srcpar=this->GetItemParent(src);
    wxTreeItemId srcnext=this->GetNextSibling(src);
    wxTreeItemId destpar=this->GetItemParent(dest);

    //We need to make sure we're not trying to append a sibling to the root:
    if(!destpar.IsOk())
    {
        wxBell();
        return wxTreeItemId();
    }

    //step 2: collect necessary info
    int srcid=GetDbItemId(src);
    int destid=GetDbItemId(dest);
    int destord=GetOrdering(destid);
    int destparid=GetDbItemId(destpar);

    //step3: add a new node after the dest item and copy the src data and
    //children to it
    this->Freeze();
    wxTreeItemId newitem = this->InsertItem(destpar, dest, wxEmptyString);

    CopyNode(src,newitem);
    MoveChildren(src,newitem);
    wxTreeCtrl::Delete(src);

    if(this->GetChildrenCount(srcpar,true)==0)
    {
        this->SetItemHasChildren(srcpar,false);
    }

    this->Thaw();

    //step 4: update the database
    BumpOrderingsAfter(destord, destparid);

    ChangeParentAndOrder(srcid,destparid,destord+1);

    if(srcnext.IsOk())
    {
        int nextno = GetDbItemId(srcnext);
        int nextord = GetOrdering(nextno);
        int parid = GetDbItemId(srcpar);

        DecOrderingsStartingAt(nextord, parid);
    }

    return newitem;
}

wxTreeItemId wxRearrangeTree::MoveAsFirstChild(const wxTreeItemId& src,const wxTreeItemId& dest)
{
    //step 1: make sure all the necessary nodes are ok
    if(!MoveChecker(src,dest))
    {
        wxBell();
        return wxTreeItemId();
    }

    wxTreeItemId srcpar=this->GetItemParent(src);
    wxTreeItemId srcnext=this->GetNextSibling(src);

    //step 2: collect necessary info
    int srcid=GetDbItemId(src);
    int destid=GetDbItemId(dest);

    //step3: add a new node after the dest item and copy the data
    this->Freeze();
    wxTreeItemId newitem = this->PrependItem(dest, wxEmptyString);
    CopyNode(src,newitem);
    MoveChildren(src,newitem);
    wxTreeCtrl::Delete(src);
    this->SetItemHasChildren(dest,true);

    if(this->GetChildrenCount(srcpar,true)==0)
    {
        this->SetItemHasChildren(srcpar,false);
    }

    this->Thaw();

    //step 4: update the database
    BumpOrderingsAfter(0, destid);

    ChangeParentAndOrder(srcid,destid,1);

    if(srcnext.IsOk())
    {
        int nextno = GetDbItemId(srcnext);
        int nextord = GetOrdering(nextno);
        int parid = GetDbItemId(srcpar);

        DecOrderingsStartingAt(nextord, parid);
    }

    return newitem;
}

wxTreeItemId wxRearrangeTree::MoveAsLastChild(const wxTreeItemId& src,const wxTreeItemId& dest)
{
    //step 1: make sure all the necessary nodes are ok
    if(!MoveChecker(src,dest))
    {
        wxBell();
        return wxTreeItemId();
    }

    wxTreeItemId srcpar=this->GetItemParent(src);
    wxTreeItemId srcnext=this->GetNextSibling(src);

    //step 2: collect necessary info
    int srcid=GetDbItemId(src);
    int destid=GetDbItemId(dest);

    //step3: add a new node after the dest item and copy the data
    this->Freeze();
    wxTreeItemId newitem = this->AppendItem(dest, wxEmptyString);

    CopyNode(src,newitem);
    MoveChildren(src,newitem);
    wxTreeCtrl::Delete(src);
    this->SetItemHasChildren(dest,true);

    if(this->GetChildrenCount(srcpar,true)==0)
    {
        this->SetItemHasChildren(srcpar,false);
    }

    this->Thaw();

    //step 4: update the database
    int neword=GetNextOrderingIn(destid);
    ChangeParentAndOrder(srcid,destid,neword);

    if(srcnext.IsOk())
    {
        int nextno = GetDbItemId(srcnext);
        int nextord = GetOrdering(nextno);
        int parid = GetDbItemId(srcpar);

        DecOrderingsStartingAt(nextord, parid);
    }

    return newitem;
}

bool wxRearrangeTree::MoveUp(const wxTreeItemId &selitem)
{
    //step 1a: quit if item is invalid
    if(!selitem.IsOk())
    {
        wxBell();
        return false;
    }

    //step 1b: get the previous sibling (and quit if there is none)
    wxTreeItemId prev = this->GetPrevSibling(selitem);
    if(!prev.IsOk())
    {
        wxBell();
        return false;
    }

    //step 1c: get the parent (and quit if there is none)
    wxTreeItemId parent = this->GetItemParent(selitem);
    if(!parent.IsOk())
    {
        wxBell();
        return false;
    }

    //step 2: get some necessary data
    int prevno = GetDbItemId(prev);
    int selno = GetDbItemId(selitem);

    //step 3: make a new item after the current one (and copy the label,icon and
    //data to it).
    this->Freeze();
    wxTreeItemId newitem = this->InsertItem(parent, selitem, "");
    CopyNode(prev,newitem);

    //step 4: "move" the children
    MoveChildren(prev,newitem);

    //step 5: delete the previous item
    wxTreeCtrl::Delete(prev);
    this->Thaw();

    //step 6: swap the orderNos in the database
    SwapOrderings(selno,prevno);

    return true;
}

bool wxRearrangeTree::MoveDown(const wxTreeItemId &selitem)
{
    //It's easy enough to move an item down by moving the next item up.
    //But in the move up method, the previous item is deleted.  So if
    //we call MoveUp(next), the item previous to the next item - ie the
    //current item - will be deleted and we'll lose the current selection.

    //To avoid this, I'll basically repeat the same code modified as necessary

    //step 1a:
    if(!selitem.IsOk())
    {
        wxBell();
        return false;
    }

    //step 1b: get the next sibling (and quit if there is none)
    wxTreeItemId next = this->GetNextSibling(selitem);
    if(!next.IsOk())
    {
        wxBell();
        return false;
    }

    //step 1c: get the parent (and quit if there is none)
    wxTreeItemId parent = this->GetItemParent(selitem);
    if(!parent.IsOk())
    {
        wxBell();
        return false;
    }

    //step 2: get some necessary data
    int selno = GetDbItemId(selitem);
    int nextno = GetDbItemId(next);

    //step 3: make a new item before the current one (and copy the label, icon,
    //and data to it).
    wxTreeItemId prev = this->GetPrevSibling(selitem);
    wxTreeItemId newitem;

    this->Freeze();
    if(prev.IsOk())
    {
        newitem = this->InsertItem(parent, prev, wxEmptyString);
    }
    else
    {
        newitem = this->InsertItem(parent, 0, wxEmptyString);
    }
    CopyNode(next,newitem);

    //step 4: "move" the childred
    MoveChildren(next,newitem);

    //step 5: delete the next item
    wxTreeCtrl::Delete(next);
    this->Thaw();

    //step 6: swap the orderNos in the database
    SwapOrderings(selno,nextno);

    return true;
}

bool wxRearrangeTree::MoveLeft(const wxTreeItemId& selitem)
{
    if(!selitem.IsOk())
    {
        wxBell();
        return false;
    }

    wxTreeItemId next = this->GetNextSibling(selitem);
    wxTreeItemId parent = this->GetItemParent(selitem);

    if(!parent.IsOk())
    {
        wxBell();
        return false;
    }

    wxTreeItemId parentparent = this->GetItemParent(parent);

    if(!parentparent.IsOk())
    {
        wxBell();
        return false;
    }

    wxTreeItemId parentsibling = this->GetNextSibling(parent);

    int selno = GetDbItemId(selitem);
    int parno = GetDbItemId(parent);
    int parparno = GetDbItemId(parentparent);

    this->Freeze();
    wxTreeItemId newitem = this->InsertItem(parentparent, parent, wxEmptyString);
    CopyNode(selitem,newitem);

    MoveChildren(selitem,newitem);

    wxTreeCtrl::Delete(selitem);
    this->SelectItem(newitem);
    this->Thaw();

    //For some reason, even if selitem was the only child of parent so that
    //deleting it leaves parent empty, parent will still have a toggle box unless
    //we do the manually set it to have no children:
    if(this->GetChildrenCount(parent)==0)
    {
        this->SetItemHasChildren(parent,false);
    }

    //Finally, update a bunch of things in the database

    //part1:if there are any items after the new item, bump all their orderNos by1
    int parord = GetOrdering(parno);
    if(parentsibling.IsOk())
    {
        BumpOrderingsAfter(parord,parparno);
    }

    //part 2: update the new item to have a its new parent and orderno
    ChangeParentAndOrder(selno,parparno,parord+1);

    //part3: if there are any items after the item we just moved, decrease
    //all their orderNos by 1
    if(next.IsOk())
    {
        int nextno=GetDbItemId(next);
        int nextord=GetOrdering(nextno);
        DecOrderingsStartingAt(nextord,parno);
    }

    return true;
}

bool wxRearrangeTree::MoveRight(const wxTreeItemId& selitem)
{
    if(!selitem.IsOk())
    {
        wxBell();
        return false;
    }

    wxTreeItemId prev = this->GetPrevSibling(selitem);

    if(!prev.IsOk())
    {
        wxBell();
        return false;
    }

    wxTreeItemId next = this->GetNextSibling(selitem);

    int selno = GetDbItemId(selitem);
    int prevno = GetDbItemId(prev);

    //if we have flagged the prev item as having children, but we haven't
    //mapped them yet, we need to map them now, so that the item will
    //will be appended in the right place.
    if( this->ItemHasChildren(prev) && this->GetChildrenCount(prev)==0 )
    {
        RestoreChildren(prev);
    }

    this->Freeze();
    wxTreeItemId newitem = this->AppendItem(prev, wxEmptyString);
    CopyNode(selitem,newitem);

    //If the item previously had children, but was made empty by a move left
    //call, the item won't get a toggle box unless we do the following:
    this->SetItemHasChildren(prev,true);

    MoveChildren(selitem,newitem);

    this->Expand(prev);
    wxTreeCtrl::Delete(selitem);
    this->SelectItem(newitem);
    this->Thaw();

    //Update the database based on the changes just made
    int newOrd = GetNextOrderingIn(prevno);
    ChangeParentAndOrder(selno,prevno,newOrd);

    if(next.IsOk())
    {
        wxTreeItemId par = this->GetItemParent(next);

        int nextno = GetDbItemId(next);
        int nextord = GetOrdering(nextno);
        int parid = GetDbItemId(par);

        DecOrderingsStartingAt(nextord, parid);
    }

    return true;
}

//overrides

void wxRearrangeTree::Delete(const wxTreeItemId &item)
{
    //We need to delete the item and all its children from the db.
    //There's probably a better way, but this works for now.

    wxTreeItemId itempar = this->GetItemParent(item);
    std::stack<int> deletes;
    std::stack<int> items;
    int cur=GetDbItemId(item);
    int ord=GetOrdering(cur);
    int par=GetParent(cur);

    items.push(cur);

    while(!items.empty())
    {
        cur=items.top();
        items.pop();
        deletes.push(cur);

        //SELECT ItemID FROM Tree WHERE Parent=1;
        wxSQLite3Statement stmt = m_db->PrepareStatement(m_getChildrenString);
        stmt.Bind(1,cur);
        wxSQLite3ResultSet result = stmt.ExecuteQuery();

        while( result.NextRow() )
        {
            items.push(result.GetInt(0));
        }
        result.Finalize();
        stmt.Finalize();
    }

    m_db->Begin();

    while(!deletes.empty())
    {
        cur=deletes.top();
        deletes.pop();

        m_deleteItemStmt.ClearBindings();
        m_deleteItemStmt.Reset();
        m_deleteItemStmt.Bind(1,cur);
        m_deleteItemStmt.ExecuteUpdate();

        if(!m_expandTableName.IsEmpty())
        {
            m_remExpandedStmt.ClearBindings();
            m_remExpandedStmt.Reset();
            m_remExpandedStmt.Bind(1,cur);
            m_remExpandedStmt.ExecuteUpdate();
        }
    }

    DecOrderingsStartingAt(ord,par);
    m_db->Commit();

    //call the base class delete to remove the node from the tree
    wxTreeCtrl::Delete(item);

    //if we just removed the last child, we need to manually remove the
    //toggle button:
    if(this->GetChildrenCount(itempar,true)==0)
    {
        this->SetItemHasChildren(itempar,false);
        RemoveFromExpanded(itempar);
    }
}

void wxRearrangeTree::DeleteAllItems()
{
    DumpAllData();
    wxTreeCtrl::DeleteAllItems();
}

void wxRearrangeTree::SetItemImage(const wxTreeItemId &item, int image, wxTreeItemIcon which)
{
    int i=GetDbItemId(item);
    int d=GetDbAuxId(item);

    m_treeHelper->UpdateIcon(i,d,image,which);
    wxTreeCtrl::SetItemImage(item,image,which);
}

void wxRearrangeTree::SetItemText(const wxTreeItemId &item, const wxString &text)
{
    int i=GetDbItemId(item);
    int d=GetDbAuxId(item);

    m_treeHelper->UpdateLabel(i,d,text);
    wxTreeCtrl::SetItemText(item,text);
}

void wxRearrangeTree::SortChildren(const wxTreeItemId &item)
{
    //first sort with the standard method
    wxTreeCtrl::SortChildren(item);

    //next we need to update all the ordinals in the db
    int parid=GetDbItemId(item);
    int i(1),itemid;

    wxTreeItemId child;
    wxTreeItemIdValue cookie;

    child = this->GetFirstChild(item,cookie);

    while( child.IsOk() )
    {
        itemid=GetDbItemId(child);
        ChangeParentAndOrder(itemid,parid,i);

        child = this->GetNextChild(item,cookie);
        i++;
    }
}

void wxRearrangeTree::RestoreChildren(const wxTreeItemId& curitem)
{
    std::queue<wxTreeItemId> items;
    std::stack<wxTreeItemId> expandeditems;
    wxTreeItemId item,t;
    int ItemID,Data,ChildrenCnt;

    items.push(curitem);

    while(!items.empty())
    {
        item=items.front();
        items.pop();
        ItemID=GetDbItemId(item);

        //"SELECT SubQ.ItemID,SubQ.Data ,(SELECT count(*) FROM %s WHERE Parent=SubQ.ItemID)"
        //"FROM "
        //"(SELECT ItemID,Ordering,Data FROM %s WHERE Parent = ? ORDER BY Ordering) AS SubQ;"

        wxSQLite3Statement stmt = m_db->PrepareStatement(m_mapChildrenString);
        stmt.Bind(1,ItemID);

        wxSQLite3ResultSet result = stmt.ExecuteQuery();

        while( result.NextRow() )
        {
            ItemID=result.GetInt(0);
            Data=result.GetInt(1);
            ChildrenCnt=result.GetInt(2);

            t = this->AppendItem(item,wxEmptyString);
            ConfigureNode(t,ItemID,Data);

            if(ChildrenCnt)
            {
                SetItemHasChildren(t);
            }

            if(!m_expandTableName.IsEmpty())
            {
                if(m_expandedItems.find(ItemID)!=m_expandedItems.end())
                {
                    items.push(t);
                    expandeditems.push(t);
                }
            }

        }
        result.Finalize();
        stmt.Finalize();
    }

    while(!expandeditems.empty())
    {
        this->Expand(expandeditems.top());
        expandeditems.pop();
    }
}

void wxRearrangeTree::MoveChildren(const wxTreeItemId& sourceRoot,const wxTreeItemId& destRoot)
{
    std::stack<std::pair<wxTreeItemId,wxTreeItemId>> mystack;
    std::stack<wxTreeItemId> expands;
    wxTreeItemId src,dest,child,newitem;
    wxTreeItemIdValue cookie;

    mystack.push( std::pair<wxTreeItemId,wxTreeItemId>(sourceRoot,destRoot) );

    while( !mystack.empty() )
    {
        src = mystack.top().first;
        dest = mystack.top().second;
        mystack.pop();

        if( this->ItemHasChildren(src) )
        {
            this->SetItemHasChildren(dest);
        }

        if( this->IsExpanded(src) )
        {
            expands.push(dest);
        }

        child = this->GetFirstChild(src,cookie);

        while( child.IsOk() )
        {
            newitem = this->AppendItem(dest,wxEmptyString);
            CopyNode(child,newitem);

            mystack.push(std::pair<wxTreeItemId,wxTreeItemId>(child,newitem));

            child = this->GetNextChild(src,cookie);
        }
    }

    while(!expands.empty())
    {
        this->Expand(expands.top());
        expands.pop();
    }
}

void wxRearrangeTree::CopyNode(const wxTreeItemId& src, const wxTreeItemId& dest)
{
    int srcno = GetDbItemId(src);
    int srcdata = GetDbAuxId(src);

    this->SetItemText(dest, this->GetItemText(src));
    this->SetItemData(dest, new rtdbTreeData(srcno,srcdata));

    for(int i=wxTreeItemIcon_Normal;i<wxTreeItemIcon_Max;++i)
    {
        this->SetItemImage(dest, this->GetItemImage(src,static_cast<wxTreeItemIcon>(i)),static_cast<wxTreeItemIcon>(i));
    }
}

void wxRearrangeTree::ConfigureNode(const wxTreeItemId& item, int i, int d)
{
    wxTreeCtrl::SetItemText(item, m_treeHelper->GetLabel(i,d));
    this->SetItemData(item,new rtdbTreeData(i,d));

    for(int j=wxTreeItemIcon_Normal;j<wxTreeItemIcon_Max;++j)
    {
        wxTreeCtrl::SetItemImage(item, m_treeHelper->GetIcon(i,d,static_cast<wxTreeItemIcon>(j)),static_cast<wxTreeItemIcon>(j));
    }
}

bool wxRearrangeTree::MoveChecker(const wxTreeItemId& src,const wxTreeItemId& dest)
{
    //step 1: make sure all the necessary nodes are ok
    if(!src.IsOk())
    {
        wxBell();
        return false;
    }

    if(!dest.IsOk())
    {
        return false;
    }

    //we also need to make sure that src is neither the same as dest nor an
    //ancestor of dest
    wxTreeItemId temp=dest;
    while(temp.IsOk())
    {
        if(temp==src)
        {
            return false;
        }

        temp=this->GetItemParent(temp);
    }

    return true;
}

void wxRearrangeTree::RemoveFromExpanded(const wxTreeItemId& item)
{
    if(!m_expandTableName.IsEmpty())
    {
        int i=GetDbItemId(item);
        std::set<int>::iterator it = m_expandedItems.find(i);

        if(it!=m_expandedItems.end())
        {
            m_expandedItems.erase(it);

            m_remExpandedStmt.ClearBindings();
            m_remExpandedStmt.Reset();
            m_remExpandedStmt.Bind(1,i);
            m_remExpandedStmt.ExecuteUpdate();
        }
    }
}

int wxRearrangeTree::GetOrdering(int i)
{
    return GetMiscItem(i,m_getOrderingString);
}

int wxRearrangeTree::GetParent(int i)
{
    return GetMiscItem(i,m_getParentString);
}

int wxRearrangeTree::GetNextOrderingIn(int i)
{
    return GetMiscItem(i,m_getNextOrdString);
}

int wxRearrangeTree::GetMiscItem(int i,const wxString& s)
{
    int value=-1;

    wxSQLite3Statement stmt = m_db->PrepareStatement(s);
    stmt.Bind(1,i);
    wxSQLite3ResultSet result = stmt.ExecuteQuery();

    if( result.NextRow() )
    {
        value = result.GetInt(0);
    }
    result.Finalize();
    stmt.Finalize();

    return value;
}

int wxRearrangeTree::NewDbItem(int Parent, int Ordering, int Data)
{
    //"INSERT INTO %s(ItemID,Parent,Ordering,Data) VALUES (null,?,?,?);"

    m_newDBItemStmt.ClearBindings();
    m_newDBItemStmt.Reset();
    m_newDBItemStmt.Bind(1,Parent);
    m_newDBItemStmt.Bind(2,Ordering);
    m_newDBItemStmt.Bind(3,Data);
    m_newDBItemStmt.ExecuteUpdate();

    return m_db->ExecuteScalar("SELECT last_insert_rowid();");
}

void wxRearrangeTree::DumpAllData()
{
    m_deleteAllItemsStmt.ExecuteUpdate();

    if(!m_expandTableName.IsEmpty())
    {
        m_deleteAllExpandedStmt.ExecuteUpdate();
    }
}

void wxRearrangeTree::SwapOrderings(int firstid,int secondid)
{
    int firstordering=GetOrdering(firstid);
    int secondordering=GetOrdering(secondid);

    m_updateOrderingStmt.ClearBindings();
    m_updateOrderingStmt.Reset();
    m_updateOrderingStmt.Bind(1,firstordering);
    m_updateOrderingStmt.Bind(2,secondid);
    m_updateOrderingStmt.ExecuteUpdate();

    m_updateOrderingStmt.ClearBindings();
    m_updateOrderingStmt.Reset();
    m_updateOrderingStmt.Bind(1,secondordering);
    m_updateOrderingStmt.Bind(2,firstid);
    m_updateOrderingStmt.ExecuteUpdate();
}

void wxRearrangeTree::BumpOrderingsAfter(int Ordering, int Parent)
{
    m_bumpOrdStmt.ClearBindings();
    m_bumpOrdStmt.Reset();
    m_bumpOrdStmt.Bind(1,Ordering);
    m_bumpOrdStmt.Bind(2,Parent);
    m_bumpOrdStmt.ExecuteUpdate();
}

void wxRearrangeTree::DecOrderingsStartingAt(int Ordering, int Parent)
{
    m_decOrdStmt.ClearBindings();
    m_decOrdStmt.Reset();
    m_decOrdStmt.Bind(1,Ordering);
    m_decOrdStmt.Bind(2,Parent);
    m_decOrdStmt.ExecuteUpdate();
}

void wxRearrangeTree::ChangeParentAndOrder(int no,int par,int ord)
{
    //UPDATE Items SET Parent=?,Ordering=? WHERE ItemID=?;

    m_updateParentAndOrderingStmt.ClearBindings();
    m_updateParentAndOrderingStmt.Reset();
    m_updateParentAndOrderingStmt.Bind(1,par);
    m_updateParentAndOrderingStmt.Bind(2,ord);
    m_updateParentAndOrderingStmt.Bind(3,no);
    m_updateParentAndOrderingStmt.ExecuteUpdate();
}

//event handlers

void wxRearrangeTree::OnTreeItemExpanding( wxTreeEvent& event )
{
    wxTreeItemId selection = event.GetItem();

    if( !this->ItemHasChildren(selection) )
    {
        return;
    }
    else if( this->GetChildrenCount(selection)!=0 )
    {
        return;
    }
    else
    {
        RestoreChildren(selection);
    }
}

void wxRearrangeTree::OnTreeItemExpanded( wxTreeEvent& event )
{
    if(!m_expandTableName.IsEmpty())
    {
        int i=GetDbItemId(event.GetItem());

        if(m_expandedItems.find(i)==m_expandedItems.end())
        {
            m_expandedItems.insert(i);

            m_addExpandedStmt.ClearBindings();
            m_addExpandedStmt.Reset();
            m_addExpandedStmt.Bind(1,i);
            m_addExpandedStmt.ExecuteUpdate();
        }
    }
}

void wxRearrangeTree::OnTreeItemCollapsed( wxTreeEvent& event )
{
    if(m_fullVirtual)
    {
        //First call collapse all children to remove any expaned children
        //from the expands table
        this->CollapseAllChildren(event.GetItem());

        //next, we need to delete all the children from the tree, but just
        //calling DeleteChildren() or even wxTreeCtrl::DeleteChildren()
        //will delete from the db as well.
        //se we need to do it in this convoluted way
        wxTreeItemId child;
        wxTreeItemIdValue cookie;

        child = this->GetFirstChild(event.GetItem(),cookie);
        std::stack<wxTreeItemId> deletes;

        while( child.IsOk() )
        {
            deletes.push(child);
            child = this->GetNextChild(event.GetItem(),cookie);
        }

        while(!deletes.empty())
        {
            child=deletes.top();
            deletes.pop();
            wxTreeCtrl::Delete(child);
        }
    }

    RemoveFromExpanded(event.GetItem());
}

void wxRearrangeTree::OnTreeEndLabelEdit( wxTreeEvent& event )
{
    if(event.IsEditCancelled())
    {
        return;
    }

    wxTreeItemId selitem = this->GetSelection();

    if(!selitem.IsOk())
    {
        return;
    }

    int i = GetDbItemId(selitem);
    int d = GetDbAuxId(selitem);

    m_treeHelper->UpdateLabel(i,d,event.GetLabel());
}


void wxRearrangeTree::OnTreeKeyUp( wxKeyEvent& event )
{
    if(event.GetKeyCode()==WXK_F2)
    {
        this->EditLabel(this->GetSelection());
    }

    event.Skip();
}
