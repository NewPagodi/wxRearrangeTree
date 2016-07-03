/***************************************************************
 * Name:      dbrearangetree3Main.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2016-07-01
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "dbrearangetree3Main.h"
#include "repview.xpm"
#include "folder.xpm"

class myRearrangeTreeHelper:public wxRearrangeTreeHelper
{
    public:
        myRearrangeTreeHelper(wxSQLite3Database*);
        wxString GetLabel(int,int);
        void UpdateLabel(int i,int d,const wxString& s);

    private:
        wxSQLite3Database* m_db;
};

myRearrangeTreeHelper::myRearrangeTreeHelper(wxSQLite3Database* d)
    :wxRearrangeTreeHelper(),m_db(d)
{

}

wxString myRearrangeTreeHelper::GetLabel(int,int d)
{
    wxString Name="New Item";

    wxSQLite3Statement stmt = m_db->PrepareStatement("SELECT Label FROM Data WHERE AuxId=?;");
    stmt.Bind(1,d);

    wxSQLite3ResultSet result = stmt.ExecuteQuery();
    if( result.NextRow() )
    {
        Name=result.GetString(0);
    }
    result.Finalize();
    stmt.Finalize();

    return Name;
}

void myRearrangeTreeHelper::UpdateLabel(int,int d,const wxString& s)
{
    wxSQLite3Statement stmt = m_db->PrepareStatement("Update Data set Label=? WHERE AuxId=?;");
    stmt.Bind(1,s);
    stmt.Bind(2,d);
    stmt.ExecuteUpdate();
}


dbrearangetree3Frame::dbrearangetree3Frame(wxFrame *frame): GUIFrame(frame)
{
    m_auinotebook1->SetArtProvider( new wxAuiGenericTabArt() );

    db.Open("data.db");

    m_dbtree->SetHelperAndData(new myRearrangeTreeHelper(&db),&db,"Tree","Expanded");
    m_dbtree->Bind( wxEVT_TREE_ITEM_EXPANDING, &dbrearangetree3Frame::OnTreeItemExpanding, this );

    wxImageList* il=new wxImageList(16, 15);
    il->Add(wxBitmap(folder_xpm));
    il->Add(wxBitmap(repview_xpm));
    m_dbtree2->AssignImageList(il);

    m_dbtree2->SetData(&db,"SimpleTree");
}

dbrearangetree3Frame::~dbrearangetree3Frame()
{
    if(db.IsOpen())
    {
        db.Close();
    }
}

void dbrearangetree3Frame::OnClose(wxCloseEvent& WXUNUSED(event))
{
    Destroy();
}

void dbrearangetree3Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Destroy();
}

void dbrearangetree3Frame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
}

void dbrearangetree3Frame::OnTreeItemExpanding( wxTreeEvent& event )
{
    //(*m_textCtrl1) << "item expanding.\n";
    SetStatusText("item expanding.",1);
    event.Skip();
}

int dbrearangetree3Frame::NewDataItem(const wxString& s)
{
    int newid=db.ExecuteScalar("SELECT max(AuxId)+1 FROM Data;");

    wxSQLite3Statement stmt = db.PrepareStatement("insert into data(AuxId,Label,Icon) VALUES (?,?,-1);");
    stmt.Bind(1,newid);
    stmt.Bind(2,s);
    stmt.ExecuteUpdate();

    return newid;
}

void dbrearangetree3Frame::OnUp(wxCommandEvent& WXUNUSED(event))
{
    if(m_auinotebook1->GetSelection()==0)
    {
        m_dbtree->MoveUp(m_dbtree->GetSelection());
    }
    else
    {
        m_dbtree2->MoveUp(m_dbtree2->GetSelection());
    }
}

void dbrearangetree3Frame::OnDown(wxCommandEvent& WXUNUSED(event))
{
    if(m_auinotebook1->GetSelection()==0)
    {
        m_dbtree->MoveDown(m_dbtree->GetSelection());
    }
    else
    {
        m_dbtree2->MoveDown(m_dbtree2->GetSelection());
    }
}

void dbrearangetree3Frame::OnLeft(wxCommandEvent& WXUNUSED(event))
{
    if(m_auinotebook1->GetSelection()==0)
    {
        m_dbtree->MoveLeft(m_dbtree->GetSelection());
    }
    else
    {
        m_dbtree2->MoveLeft(m_dbtree2->GetSelection());
    }
}

void dbrearangetree3Frame::OnRight(wxCommandEvent& WXUNUSED(event))
{
    if(m_auinotebook1->GetSelection()==0)
    {
        m_dbtree->MoveRight(m_dbtree->GetSelection());
    }
    else
    {
        m_dbtree2->MoveRight(m_dbtree2->GetSelection());
    }
}

void dbrearangetree3Frame::OnNewSib(wxCommandEvent& WXUNUSED(event))
{
    if(m_auinotebook1->GetSelection()==0)
    {
        wxTreeItemId id=m_dbtree->GetSelection();

        if(id.IsOk())
        {
            int newdata=NewDataItem("New Sibling");
            wxTreeItemId newitem=m_dbtree->NewNextSibling(id,newdata);

            if(newitem.IsOk())
            {
                m_dbtree->SelectItem(newitem);
            }
        }
    }
    else
    {
        m_dbtree2->NewNextSibling(m_dbtree2->GetSelection(), "New Item", 1,  0);
    }

}

void dbrearangetree3Frame::OnNewFirstChild(wxCommandEvent& WXUNUSED(event))
{
    if(m_auinotebook1->GetSelection()==0)
    {
        wxTreeItemId id=m_dbtree->GetSelection();

        if(id.IsOk())
        {
            int newdata=NewDataItem("New First Child");
            wxTreeItemId newitem=m_dbtree->NewFirstChild(id,newdata);

            if(newitem.IsOk())
            {
                m_dbtree->Expand(id);
                m_dbtree->SelectItem(newitem);
            }
        }
    }
    else
    {
        m_dbtree2->NewFirstChild(m_dbtree2->GetSelection(), "New Item", 1,  0);
    }
}

void dbrearangetree3Frame::OnNewLastChild(wxCommandEvent& WXUNUSED(event))
{
    if(m_auinotebook1->GetSelection()==0)
    {
        wxTreeItemId id=m_dbtree->GetSelection();

        if(id.IsOk())
        {
            int newdata=NewDataItem("New Last Child");
            wxTreeItemId newitem=m_dbtree->NewLastChild(id,newdata);

            if(newitem.IsOk())
            {
                m_dbtree->Expand(id);
                m_dbtree->SelectItem(newitem);
            }
        }
    }
    else
    {
        m_dbtree2->NewLastChild(m_dbtree2->GetSelection(), "New Item", 1,  0);
    }
}

void dbrearangetree3Frame::OnDelete(wxCommandEvent& WXUNUSED(event))
{
    if(m_auinotebook1->GetSelection()==0)
    {
        m_dbtree->Delete(m_dbtree->GetSelection());
    }
    else
    {
        m_dbtree2->Delete(m_dbtree2->GetSelection());
    }
}

void dbrearangetree3Frame::OnMark(wxCommandEvent& WXUNUSED(event))
{
    if(m_auinotebook1->GetSelection()==0)
    {
        wxTreeItemId id=m_dbtree->GetSelection();

        if(id.IsOk())
        {
            if(m_marked.IsOk())
            {
                m_dbtree->SetItemTextColour(m_marked,*wxBLACK);
            }

            m_dbtree->SetItemTextColour(id,*wxRED);

            m_marked=id;
        }

        EnableButtons(m_marked.IsOk());
    }
    else
    {
        wxTreeItemId id=m_dbtree2->GetSelection();

        if(id.IsOk())
        {
            if(m_markedSimple.IsOk())
            {
                m_dbtree2->SetItemTextColour(m_markedSimple,*wxBLACK);
            }

            m_dbtree2->SetItemTextColour(id,*wxRED);

            m_markedSimple=id;
        }

        EnableButtons(m_markedSimple.IsOk());
    }
}

void dbrearangetree3Frame::OnMoveAfter(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId newitem;

    if(m_auinotebook1->GetSelection()==0)
    {
        wxTreeItemId id=m_dbtree->GetSelection();

        if(id.IsOk())
        {
            if(m_marked.IsOk())
            {
                newitem=m_dbtree->MoveAfter(m_marked,id);

                if(newitem.IsOk())
                {
                    m_marked=wxTreeItemId();
                    m_dbtree->SelectItem(newitem);
                    EnableButtons(false);
                }
            }
        }
    }
    else
    {
        wxTreeItemId id=m_dbtree2->GetSelection();

        if(id.IsOk())
        {
            if(m_markedSimple.IsOk())
            {
                newitem=m_dbtree2->MoveAfter(m_markedSimple,id);

                if(newitem.IsOk())
                {
                    m_markedSimple=wxTreeItemId();
                }
            }
        }
    }
}

void dbrearangetree3Frame::OnMoveAsFirstChld(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId newitem;

    if(m_auinotebook1->GetSelection()==0)
    {
        wxTreeItemId id=m_dbtree->GetSelection();

        if(id.IsOk())
        {
            if(m_marked.IsOk())
            {
                newitem=m_dbtree->MoveAsFirstChild(m_marked,id);

                if(newitem.IsOk())
                {
                    m_marked=wxTreeItemId();

                    m_dbtree->Expand(id);
                    m_dbtree->SelectItem(newitem);
                    EnableButtons(false);
                }
            }
        }
    }
    else
    {
        wxTreeItemId id=m_dbtree2->GetSelection();

        if(id.IsOk())
        {
            if(m_markedSimple.IsOk())
            {
                newitem=m_dbtree2->MoveAsFirstChild(m_markedSimple,id);

                if(newitem.IsOk())
                {
                    m_markedSimple=wxTreeItemId();
                }
            }
        }
    }
}

void dbrearangetree3Frame::OnMoveAsLastChild(wxCommandEvent& WXUNUSED(event))
{
    wxTreeItemId newitem;

    if(m_auinotebook1->GetSelection()==0)
    {
        wxTreeItemId id=m_dbtree->GetSelection();

        if(id.IsOk())
        {
            if(m_marked.IsOk())
            {
                newitem=m_dbtree->MoveAsLastChild(m_marked,id);

                if(newitem.IsOk())
                {
                    m_marked=wxTreeItemId();

                    m_dbtree->Expand(id);
                    m_dbtree->SelectItem(newitem);
                    EnableButtons(false);
                }
            }
        }
    }
    else
    {
        wxTreeItemId id=m_dbtree2->GetSelection();

        if(id.IsOk())
        {
            if(m_markedSimple.IsOk())
            {
                newitem=m_dbtree2->MoveAsLastChild(m_markedSimple,id);

                if(newitem.IsOk())
                {
                    m_markedSimple=wxTreeItemId();
                }
            }
        }
    }
}

void dbrearangetree3Frame::OnSetFullVirtual( wxCommandEvent& WXUNUSED(event) )
{
    if(m_auinotebook1->GetSelection()==0)
    {
        m_dbtree->SetFullVitual(true);
    }
    else
    {
        m_dbtree2->SetFullVitual(true);
    }
}

void dbrearangetree3Frame::OnCount( wxCommandEvent& WXUNUSED(event) )
{
    int count;

    if(m_auinotebook1->GetSelection()==0)
    {
        count=m_dbtree->GetCount();
    }
    else
    {
        count=m_dbtree2->GetCount();
    }


    statusBar->SetStatusText(wxString::Format("%d items",count), 1);
}

void dbrearangetree3Frame::OnNotebookPageChanged( wxAuiNotebookEvent& event )
{
    if(event.GetSelection()==0)
    {
        EnableButtons(m_marked.IsOk());
    }
    else
    {
        EnableButtons(m_markedSimple.IsOk());
    }
}

void dbrearangetree3Frame::EnableButtons(bool b)
{
    m_button6->Enable(b);
    m_button7->Enable(b);
    m_button8->Enable(b);
}
