/***************************************************************
 * Name:      dbrearangetree3Main.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2016-07-01
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef DBREARANGETREE3MAIN_H
#define DBREARANGETREE3MAIN_H

#include <wx/wxsqlite3.h>
#include "GUIFrame.h"

class dbrearangetree3Frame: public GUIFrame
{
    public:
        dbrearangetree3Frame(wxFrame *frame);
        ~dbrearangetree3Frame();
    private:
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);

        void OnTreeItemExpanding( wxTreeEvent& event );

        void OnUp(wxCommandEvent& event);
        void OnDown(wxCommandEvent& event);
        void OnLeft(wxCommandEvent& event);
        void OnRight(wxCommandEvent& event);

        void OnNewSib(wxCommandEvent& event);
        void OnNewFirstChild(wxCommandEvent& event);
        void OnNewLastChild(wxCommandEvent& event);
        void OnDelete(wxCommandEvent& event);

        void OnMark(wxCommandEvent& event);
        void OnMoveAfter(wxCommandEvent& event);
        void OnMoveAsFirstChld(wxCommandEvent& event);
        void OnMoveAsLastChild(wxCommandEvent& event);

		void OnSetFullVirtual( wxCommandEvent& event );
		void OnCount( wxCommandEvent& event );

		void OnNotebookPageChanged( wxAuiNotebookEvent& event );

		void EnableButtons(bool);
        int NewDataItem(const wxString&);

        wxSQLite3Database db;
        wxTreeItemId m_marked;
        wxTreeItemId m_markedSimple;
};

#endif // DBREARANGETREE3MAIN_H
