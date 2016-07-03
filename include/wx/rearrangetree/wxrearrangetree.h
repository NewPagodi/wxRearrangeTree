/***************************************************************
 * Name:
 * Purpose:
 * Author:     ()
 * Created:   2016-07-03
 * Copyright:  ()
 * License:   wxWindows licence
 **************************************************************/

#ifndef _WX_REARRANGETREE_REARRANGETREE_H_
#define _WX_REARRANGETREE_REARRANGETREE_H_

#include <wx/panel.h>
#include <wx/treectrl.h>
#include <wx/wxsqlite3.h>
#include <set>

#ifdef WXUSING_REARRANGETREE_SOURCE
    #define WXDLLIMPEXP_REARRANGETREE
#elif WXMAKINGDLL_REARRANGETREE
    #define WXDLLIMPEXP_REARRANGETREE WXEXPORT
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_REARRANGETREE WXIMPORT
#else // not making nor using DLL
    #define WXDLLIMPEXP_REARRANGETREE
#endif

const wxString wxRearrangeTreeNameStr = "wxRearrangeTree";
const wxString wxSimpleRearrangeTreeNameStr = "wxSimpleRearrangeTree";

class WXDLLIMPEXP_REARRANGETREE wxRearrangeTreeHelper
{
    public:
        wxRearrangeTreeHelper(){}
        virtual ~wxRearrangeTreeHelper(){}
        virtual wxString GetLabel(int i,int d){return wxString::Format("Item (%d,%d)",i,d);}
        virtual int GetIcon(int,int,wxTreeItemIcon){return -1;}
        virtual void UpdateLabel(int,int,const wxString&){}
        virtual void UpdateIcon(int,int,int,wxTreeItemIcon){}
        virtual bool IsOk(){return true;}
};

class WXDLLIMPEXP_REARRANGETREE wxRearrangeTree: public wxTreeCtrl
{
    public:
        wxRearrangeTree();
        wxRearrangeTree(wxWindow *parent, wxWindowID id=wxID_ANY, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=0, const wxString &name=wxRearrangeTreeNameStr);
        virtual ~wxRearrangeTree();
        bool Create(wxWindow *parent, wxWindowID id=wxID_ANY, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=0, const wxString &name=wxRearrangeTreeNameStr);

        bool SetHelperAndData(wxRearrangeTreeHelper*, wxSQLite3Database*, const wxString&, const wxString& e=wxEmptyString);
        void SetFullVitual(bool b);
        bool GetFullVirtual();

        wxTreeItemId NewRoot(int);
        wxTreeItemId NewNextSibling(const wxTreeItemId&, int);
        wxTreeItemId NewFirstChild(const wxTreeItemId&, int);
        wxTreeItemId NewLastChild(const wxTreeItemId&, int);

        wxTreeItemId MoveAfter(const wxTreeItemId&, const wxTreeItemId&);
        wxTreeItemId MoveAsFirstChild(const wxTreeItemId&, const wxTreeItemId&);
        wxTreeItemId MoveAsLastChild(const wxTreeItemId&, const wxTreeItemId&);

        bool MoveUp(const wxTreeItemId&);
        bool MoveDown(const wxTreeItemId&);
        bool MoveLeft(const wxTreeItemId&);
        bool MoveRight(const wxTreeItemId&);

        //overrides
        virtual void Delete(const wxTreeItemId &item);
        virtual void DeleteAllItems();
        virtual void SetItemImage(const wxTreeItemId &item, int image, wxTreeItemIcon which=wxTreeItemIcon_Normal);
        virtual void SetItemText(const wxTreeItemId &item, const wxString &text);
        virtual void SortChildren (const wxTreeItemId &item);

    protected:
        void Init();

        //tree handling methods
        int GetDbAuxId(const wxTreeItemId&);
        int GetDbItemId(const wxTreeItemId&);

        void RestoreChildren(const wxTreeItemId&);
        void MoveChildren(const wxTreeItemId&,const wxTreeItemId&);
        void CopyNode(const wxTreeItemId&,const wxTreeItemId&);
        void ConfigureNode(const wxTreeItemId&,int,int);
        bool MoveChecker(const wxTreeItemId&,const wxTreeItemId&);
        void RemoveFromExpanded(const wxTreeItemId&);

        //db handling methods:
        int GetMiscItem(int,const wxString&);
        int GetOrdering(int);
        int GetParent(int);
        int GetNextOrderingIn(int);
        int NewDbItem(int Parent, int Ordering, int Data);
        void DumpAllData();
        void SwapOrderings(int,int);
        void BumpOrderingsAfter(int no, int par);
        void DecOrderingsStartingAt(int no, int par);
        void ChangeParentAndOrder(int no,int par,int ord);

    private:
        //methods made private to help preserve sync between tree and db
        using wxTreeCtrl::AddRoot;
        using wxTreeCtrl::AppendItem;
        using wxTreeCtrl::InsertItem;
        using wxTreeCtrl::PrependItem;
        using wxTreeCtrl::SetItemData;

        //event handlers
		void OnTreeItemExpanding( wxTreeEvent& event );
		void OnTreeItemExpanded( wxTreeEvent& event );
		void OnTreeItemCollapsed( wxTreeEvent& event );
        void OnTreeEndLabelEdit( wxTreeEvent& event );
		void OnTreeKeyUp( wxKeyEvent& event );

        wxString m_getOrderingString;
        wxString m_getParentString;
        wxString m_getNextOrdString;
        wxString m_mapChildrenString;
        wxString m_getChildrenString;

        wxSQLite3Statement m_bumpOrdStmt;
        wxSQLite3Statement m_decOrdStmt;
        wxSQLite3Statement m_newDBItemStmt;
        wxSQLite3Statement m_updateOrderingStmt;
        wxSQLite3Statement m_updateParentAndOrderingStmt;
        wxSQLite3Statement m_deleteItemStmt;
        wxSQLite3Statement m_addExpandedStmt;
        wxSQLite3Statement m_remExpandedStmt;
        wxSQLite3Statement m_deleteAllExpandedStmt;
        wxSQLite3Statement m_deleteAllItemsStmt;

        bool m_fullVirtual;
        wxString m_tableName;
        wxString m_expandTableName;
        wxRearrangeTreeHelper* m_treeHelper;
        wxSQLite3Database* m_db;
        std::set<int> m_expandedItems;
};

class WXDLLIMPEXP_REARRANGETREE wxSimpleRearrangeTree: public wxRearrangeTree
{
    public:
        wxSimpleRearrangeTree();
        wxSimpleRearrangeTree(wxWindow *parent, wxWindowID id=wxID_ANY, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=0, const wxString &name=wxSimpleRearrangeTreeNameStr);
        virtual ~wxSimpleRearrangeTree();
        bool Create(wxWindow *parent, wxWindowID id=wxID_ANY, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=0, const wxString &name=wxSimpleRearrangeTreeNameStr);

        bool SetData( wxSQLite3Database*, const wxString&, const wxString& e=wxEmptyString);

        wxTreeItemId NewNextSibling(const wxTreeItemId&, const wxString&, int,  int);
        wxTreeItemId NewFirstChild(const wxTreeItemId&, const wxString&, int,  int);
        wxTreeItemId NewLastChild(const wxTreeItemId&, const wxString&, int,  int);

    private:
        using wxRearrangeTree::SetHelperAndData;
        using wxRearrangeTree::NewNextSibling;
        using wxRearrangeTree::NewFirstChild;
        using wxRearrangeTree::NewLastChild;

        void ConfigureNewItem(const wxTreeItemId&, const wxString&, int);
};

#endif // _WX_REARRANGETREE_REARRANGETREE_H_
