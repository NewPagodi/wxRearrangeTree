///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 19 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUIFRAME_H__
#define __GUIFRAME_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/rearrangetree/wxrearrangetree.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/aui/auibook.h>
#include <wx/button.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define idMenuQuit 1000
#define idMenuAbout 1001

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrame
///////////////////////////////////////////////////////////////////////////////
class GUIFrame : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* mbar;
		wxMenu* fileMenu;
		wxMenu* helpMenu;
		wxStatusBar* statusBar;
		wxPanel* m_panel2;
		wxAuiNotebook* m_auinotebook1;
		wxPanel* m_panel3;
		wxRearrangeTree* m_dbtree;
		wxPanel* m_panel4;
		wxSimpleRearrangeTree* m_dbtree2;
		wxButton* m_button1;
		wxButton* m_button2;
		wxButton* m_button3;
		wxButton* m_button4;
		wxButton* m_button9;
		wxButton* m_button10;
		wxButton* m_button11;
		wxButton* m_button12;
		wxButton* m_button5;
		wxButton* m_button6;
		wxButton* m_button7;
		wxButton* m_button8;
		wxButton* m_button13;
		wxButton* m_button14;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNotebookPageChanged( wxAuiNotebookEvent& event ) { event.Skip(); }
		virtual void OnUp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDown( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLeft( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRight( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNewSib( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNewFirstChild( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNewLastChild( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMark( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMoveAfter( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMoveAsFirstChld( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMoveAsLastChild( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSetFullVirtual( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCount( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GUIFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("wxRearrangeTree Sample"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 481,466 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~GUIFrame();
	
};

#endif //__GUIFRAME_H__
