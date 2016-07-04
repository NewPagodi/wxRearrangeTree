/***************************************************************
 * Name:
 * Purpose:
 * Author:     ()
 * Created:   2016-07-03
 * Copyright:  ()
 * License:   wxWindows licence
 **************************************************************/

#include <wx/rearrangetree/wxrearrangetree.h>

class wxSimpleRearrangeTreeHelper:public wxRearrangeTreeHelper
{
    public:
        wxSimpleRearrangeTreeHelper(wxSQLite3Database*,const wxString&);
        virtual ~wxSimpleRearrangeTreeHelper();
        virtual wxString GetLabel(int i,int d);
        virtual int GetIcon(int,int,wxTreeItemIcon);
        virtual void UpdateLabel(int,int,const wxString&);
        virtual void UpdateIcon(int i,int d,int im,wxTreeItemIcon);
        virtual bool IsOk();
    private:
        wxSQLite3Database* m_db;
        wxString m_getLabelString;
        wxString m_getIconString;
        wxString m_tableName;

        wxSQLite3Statement m_updateLabelStmt;
        wxSQLite3Statement m_updateIconStmt;
        int m_lastIconFetched;
        int m_lastIdChecked;
        bool m_ok;
};

wxSimpleRearrangeTreeHelper::wxSimpleRearrangeTreeHelper(wxSQLite3Database* d,const wxString& s)
    :wxRearrangeTreeHelper(),m_db(d),m_tableName(s),m_ok(false),m_lastIdChecked(-1)
{
    try
    {
        m_getLabelString=wxString::Format(
            "SELECT Label FROM %s WHERE ItemID=?;"
            ,m_tableName);

        m_getIconString=wxString::Format(
            "SELECT Icon FROM %s WHERE ItemID=?;"
            ,m_tableName);

        wxString st=wxString::Format(
            "UPDATE %s SET Label=? WHERE ItemId=?;"
            ,m_tableName);
        m_updateLabelStmt=m_db->PrepareStatement(st);

        st=wxString::Format(
            "UPDATE %s SET Icon=? WHERE ItemId=?;"
            ,m_tableName);
        m_updateIconStmt=m_db->PrepareStatement(st);
    }
    catch(wxSQLite3Exception& WXUNUSED(e))
    {
        //return now so that m_ok remains false.  We should
        //also probably assert or something.
        return;
    }

    m_ok=true;
}

wxSimpleRearrangeTreeHelper::~wxSimpleRearrangeTreeHelper()
{
    m_updateLabelStmt.Finalize();
    m_updateIconStmt.Finalize();
}


wxString wxSimpleRearrangeTreeHelper::GetLabel(int i,int WXUNUSED(d))
{
    wxString s = "New Item";

    wxSQLite3Statement stmt = m_db->PrepareStatement(m_getLabelString);
    stmt.Bind(1,i);

    wxSQLite3ResultSet result = stmt.ExecuteQuery();

    if( result.NextRow() )
    {
        s=result.GetString(0);
    }
    result.Finalize();
    stmt.Finalize();

    return s;
}

int wxSimpleRearrangeTreeHelper::GetIcon( int i, int WXUNUSED(d), wxTreeItemIcon WXUNUSED(ti) )
{
    //in the configure node method, this function is called 4 times.  So that
    //we don't have to do 4 db reads, we'll cache the last id checked and
    //the icon returned for it.

    //note that we're completely ignoring the wxTreeItemIcon ti parameter,
    //we only track 1 icon and use it for all states:
    if(i==m_lastIdChecked)
    {
        return m_lastIconFetched;
    }

    m_lastIdChecked=i;
    m_lastIconFetched = -1;

    wxSQLite3Statement stmt = m_db->PrepareStatement(m_getIconString);
    stmt.Bind(1,i);
    wxSQLite3ResultSet result = stmt.ExecuteQuery();

    if( result.NextRow() )
    {
        m_lastIconFetched=result.GetInt(0);
    }
    result.Finalize();
    stmt.Finalize();

    return m_lastIconFetched;
}

void wxSimpleRearrangeTreeHelper::UpdateLabel(int i,int WXUNUSED(a) ,const wxString& s)
{
    m_updateLabelStmt.ClearBindings();
    m_updateLabelStmt.Reset();
    m_updateLabelStmt.Bind(1,s);
    m_updateLabelStmt.Bind(2,i);
    m_updateLabelStmt.ExecuteUpdate();
}

void wxSimpleRearrangeTreeHelper::UpdateIcon(int i,int WXUNUSED(d),int im, wxTreeItemIcon ti)
{
    //invalidate m_lastIdChecked so that if we are asked for its icon, we
    //will refetch it instead of giving the cached value
    m_lastIdChecked=-1;

    //we only record the normal icon, all others are ignored
    if(ti!=wxTreeItemIcon_Normal)
    {
        return;
    }

    m_updateIconStmt.ClearBindings();
    m_updateIconStmt.Reset();
    m_updateIconStmt.Bind(1,im);
    m_updateIconStmt.Bind(2,i);
    m_updateIconStmt.ExecuteUpdate();
}

bool wxSimpleRearrangeTreeHelper::IsOk()
{
    return m_ok;
}

wxSimpleRearrangeTree::wxSimpleRearrangeTree()
{

}

wxSimpleRearrangeTree::wxSimpleRearrangeTree(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
{
    Create(parent, id, pos, size, style, name);
}

wxSimpleRearrangeTree::~wxSimpleRearrangeTree()
{

}

bool wxSimpleRearrangeTree::Create(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
{
    return wxRearrangeTree::Create(parent, id, pos, size, style,name);
}

bool wxSimpleRearrangeTree::SetData( wxSQLite3Database* d, const wxString& s, const wxString& e)
{
    wxSimpleRearrangeTreeHelper* h = new wxSimpleRearrangeTreeHelper(d,s);

    if(!h->IsOk())
    {
        delete h;
        return false;
    }

    return wxRearrangeTree::SetHelperAndData(h,d,s,e);
}

wxTreeItemId wxSimpleRearrangeTree::NewNextSibling(const wxTreeItemId& item, const wxString& s, int icon,  int aux)
{
    this->Freeze();
    wxTreeItemId newitem = NewNextSibling(item, aux);
    ConfigureNewItem(newitem,s,icon);
    this->Thaw();

    return newitem;
}

wxTreeItemId wxSimpleRearrangeTree::NewFirstChild(const wxTreeItemId& item, const wxString& s, int icon,  int aux)
{
    this->Freeze();
    wxTreeItemId newitem = NewFirstChild(item, aux);
    ConfigureNewItem(newitem,s,icon);
    this->Thaw();

    return newitem;
}

wxTreeItemId wxSimpleRearrangeTree::NewLastChild(const wxTreeItemId& item, const wxString& s, int icon,  int aux)
{
    this->Freeze();
    wxTreeItemId newitem = NewLastChild(item, aux);
    ConfigureNewItem(newitem,s,icon);
    this->Thaw();

    return newitem;
}

void wxSimpleRearrangeTree::ConfigureNewItem(const wxTreeItemId& newitem, const wxString& s, int icon)
{
    if(newitem.IsOk())
    {
        this->SetItemText(newitem,s);
        this->SetItemImage(newitem,icon,wxTreeItemIcon_Normal);
        //the node was configured when it was created.
        //unfortunately, we hadn't set the icons until the SetItemImage call
        //above, and so it probably got configured with the wrong icons.
        //To make sure they're right, we'll reset them now:
        for(int j=wxTreeItemIcon_Selected ;j<wxTreeItemIcon_Max;++j)
        {
            wxTreeCtrl::SetItemImage(newitem, icon,static_cast<wxTreeItemIcon>(j));
        }
    }
}
