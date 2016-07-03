///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 19 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "GUIFrame.h"

///////////////////////////////////////////////////////////////////////////

GUIFrame::GUIFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	mbar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* menuFileQuit;
	menuFileQuit = new wxMenuItem( fileMenu, idMenuQuit, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Alt+F4"), wxT("Quit the application"), wxITEM_NORMAL );
	fileMenu->Append( menuFileQuit );
	
	mbar->Append( fileMenu, wxT("&File") ); 
	
	helpMenu = new wxMenu();
	wxMenuItem* menuHelpAbout;
	menuHelpAbout = new wxMenuItem( helpMenu, idMenuAbout, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxT("Show info about this application"), wxITEM_NORMAL );
	helpMenu->Append( menuHelpAbout );
	
	mbar->Append( helpMenu, wxT("&Help") ); 
	
	this->SetMenuBar( mbar );
	
	statusBar = this->CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTATIC_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_auinotebook1 = new wxAuiNotebook( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel3 = new wxPanel( m_auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	m_dbtree = new wxRearrangeTree(m_panel3,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT|wxTR_EDIT_LABELS);
	bSizer10->Add( m_dbtree, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel3->SetSizer( bSizer10 );
	m_panel3->Layout();
	bSizer10->Fit( m_panel3 );
	m_auinotebook1->AddPage( m_panel3, wxT("Standard"), true, wxNullBitmap );
	m_panel4 = new wxPanel( m_auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_dbtree2 = new wxSimpleRearrangeTree(m_panel4,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT|wxTR_EDIT_LABELS);
	bSizer11->Add( m_dbtree2, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel4->SetSizer( bSizer11 );
	m_panel4->Layout();
	bSizer11->Fit( m_panel4 );
	m_auinotebook1->AddPage( m_panel4, wxT("Simple"), false, wxNullBitmap );
	
	bSizer9->Add( m_auinotebook1, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button1 = new wxButton( m_panel2, wxID_ANY, wxT("Up"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button1, 0, wxALL, 5 );
	
	m_button2 = new wxButton( m_panel2, wxID_ANY, wxT("Down"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button2, 0, wxALL, 5 );
	
	m_button3 = new wxButton( m_panel2, wxID_ANY, wxT("Left"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button3, 0, wxALL, 5 );
	
	m_button4 = new wxButton( m_panel2, wxID_ANY, wxT("Right"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button4, 0, wxALL, 5 );
	
	
	bSizer4->Add( bSizer3, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button9 = new wxButton( m_panel2, wxID_ANY, wxT("New Sib"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_button9, 0, wxALL, 5 );
	
	m_button10 = new wxButton( m_panel2, wxID_ANY, wxT("New First Child"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_button10, 0, wxALL, 5 );
	
	m_button11 = new wxButton( m_panel2, wxID_ANY, wxT("New Last Child"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_button11, 0, wxALL, 5 );
	
	m_button12 = new wxButton( m_panel2, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_button12, 0, wxALL, 5 );
	
	
	bSizer4->Add( bSizer6, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button5 = new wxButton( m_panel2, wxID_ANY, wxT("Mark"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_button5, 0, wxALL, 5 );
	
	m_button6 = new wxButton( m_panel2, wxID_ANY, wxT("Move After"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button6->Enable( false );
	
	bSizer5->Add( m_button6, 0, wxALL, 5 );
	
	m_button7 = new wxButton( m_panel2, wxID_ANY, wxT("Move As First Child"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button7->Enable( false );
	
	bSizer5->Add( m_button7, 0, wxALL, 5 );
	
	m_button8 = new wxButton( m_panel2, wxID_ANY, wxT("Move As Last Child "), wxDefaultPosition, wxDefaultSize, 0 );
	m_button8->Enable( false );
	
	bSizer5->Add( m_button8, 0, wxALL, 5 );
	
	
	bSizer4->Add( bSizer5, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button13 = new wxButton( m_panel2, wxID_ANY, wxT("Set Full Vitual"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_button13, 0, wxALL, 5 );
	
	m_button14 = new wxButton( m_panel2, wxID_ANY, wxT("Count"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_button14, 0, wxALL, 5 );
	
	
	bSizer4->Add( bSizer8, 1, wxEXPAND, 5 );
	
	
	bSizer9->Add( bSizer4, 0, wxEXPAND, 5 );
	
	
	m_panel2->SetSizer( bSizer9 );
	m_panel2->Layout();
	bSizer9->Fit( m_panel2 );
	bSizer1->Add( m_panel2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Connect( menuFileQuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Connect( menuHelpAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	m_auinotebook1->Connect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler( GUIFrame::OnNotebookPageChanged ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnUp ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnDown ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnLeft ), NULL, this );
	m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnRight ), NULL, this );
	m_button9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnNewSib ), NULL, this );
	m_button10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnNewFirstChild ), NULL, this );
	m_button11->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnNewLastChild ), NULL, this );
	m_button12->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnDelete ), NULL, this );
	m_button5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnMark ), NULL, this );
	m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnMoveAfter ), NULL, this );
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnMoveAsFirstChld ), NULL, this );
	m_button8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnMoveAsLastChild ), NULL, this );
	m_button13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnSetFullVirtual ), NULL, this );
	m_button14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnCount ), NULL, this );
}

GUIFrame::~GUIFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Disconnect( idMenuQuit, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Disconnect( idMenuAbout, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	m_auinotebook1->Disconnect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler( GUIFrame::OnNotebookPageChanged ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnUp ), NULL, this );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnDown ), NULL, this );
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnLeft ), NULL, this );
	m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnRight ), NULL, this );
	m_button9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnNewSib ), NULL, this );
	m_button10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnNewFirstChild ), NULL, this );
	m_button11->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnNewLastChild ), NULL, this );
	m_button12->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnDelete ), NULL, this );
	m_button5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnMark ), NULL, this );
	m_button6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnMoveAfter ), NULL, this );
	m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnMoveAsFirstChld ), NULL, this );
	m_button8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnMoveAsLastChild ), NULL, this );
	m_button13->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnSetFullVirtual ), NULL, this );
	m_button14->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIFrame::OnCount ), NULL, this );
	
}
