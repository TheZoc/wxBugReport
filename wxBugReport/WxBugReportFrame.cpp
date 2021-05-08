#include "WxBugReportFrame.h"
#include <wx/app.h>
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/event.h>

wxDEFINE_EVENT(wxEVT_COMMAND_HTML_SET_CONTENT, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_COMMAND_HTML_LOAD_PAGE, wxThreadEvent);

WxBugReportFrame::WxBugReportFrame(wxWindow * parent, wxWindowID id, const wxString & title, const wxPoint & pos, const wxSize & size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	wxImage::SetDefaultLoadFlags(wxImage::GetDefaultLoadFlags() & ~wxImage::Load_Verbose);

	this->SetSizeHints( wxSize( 500,300 ), wxDefaultSize );
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));

	// GridBagSizer for the whole frame
	wxGridBagSizer* gridBagSizerFrame;
	gridBagSizerFrame = new wxGridBagSizer( 0, 0 );
	gridBagSizerFrame->SetFlexibleDirection( wxBOTH );
	gridBagSizerFrame->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	gridBagSizerFrame->SetMinSize( wxSize( 800,600 ) ); 

	// GridBagSizer For the "Body"  (HTML Window, etc.)
	wxGridBagSizer* gridBagSizerBody;
	gridBagSizerBody = new wxGridBagSizer( 0, 0 );
	gridBagSizerBody->SetFlexibleDirection( wxBOTH );
	gridBagSizerBody->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	// HTML Viewer
	m_htmlWin = wxWebView::New(this, wxID_ANY, wxWebViewDefaultURLStr, wxDefaultPosition, wxDefaultSize, wxWebViewBackendDefault, wxSIMPLE_BORDER);
	m_htmlWin->SetMinSize( wxSize( 400,300 ) );
	gridBagSizerBody->Add( m_htmlWin, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );

	// GridBagSizer for the right side of the window
	wxGridBagSizer* gridBagSizerRight;
	gridBagSizerRight = new wxGridBagSizer(0, 0);
	gridBagSizerRight->SetFlexibleDirection(wxBOTH);
	gridBagSizerRight->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	// Add a spacer, for empty space, at the left side of the close button
	gridBagSizerRight->Add(0, 0, wxGBPosition(0, 0), wxGBSpan(1, 1), wxEXPAND, 5);

	m_btnClose = new wxButton(this, wxID_ANY, "Close", wxDefaultPosition, wxDefaultSize);
	m_btnClose->Enable(true);

	gridBagSizerRight->Add(m_btnClose, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);
	gridBagSizerRight->AddGrowableCol(0);
	gridBagSizerRight->AddGrowableRow(0);

	gridBagSizerBody->Add(gridBagSizerRight, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALIGN_RIGHT | wxEXPAND, 5);
	gridBagSizerBody->AddGrowableCol(1);
	gridBagSizerBody->AddGrowableRow(0);

	gridBagSizerFrame->Add(gridBagSizerBody, wxGBPosition(0, 0), wxGBSpan(1, 2), wxEXPAND, 5);

	this->SetSizer(gridBagSizerFrame);
	this->Layout();

	this->Centre(wxBOTH);

	// Bind Message Events
	Bind(wxEVT_COMMAND_HTML_SET_CONTENT, &WxBugReportFrame::OnHTMLSetContent, this);
	Bind(wxEVT_COMMAND_HTML_LOAD_PAGE, &WxBugReportFrame::OnHTMLLoadPage, this);
	Bind(wxEVT_CLOSE_WINDOW, &WxBugReportFrame::OnClose, this);
	Bind(wxEVT_WEBVIEW_NAVIGATING, &WxBugReportFrame::OnClickLink, this);


	// Bind Button Events
	m_btnClose->Bind(wxEVT_BUTTON, &WxBugReportFrame::OnCloseButtonClicked, this);
}

WxBugReportFrame::~WxBugReportFrame()
{
}

void WxBugReportFrame::OnCloseButtonClicked(wxCommandEvent & WXUNUSED)
{
	wxQueueEvent(wxTheApp->GetTopWindow()->GetEventHandler(), new wxCloseEvent(wxEVT_CLOSE_WINDOW));
}

void WxBugReportFrame::OnClickLink(wxWebViewEvent & evt)
{
	// Open the link on the default browser
	wxLaunchDefaultBrowser(evt.GetURL());

	// Stop navigation
	evt.Veto();
}

/*
void WxBugReportFrame::PaintEvent(wxPaintEvent & evt)
{
	wxAutoBufferedPaintDC dc(this);
	RenderFrame(dc);
}
*/

void WxBugReportFrame::RenderFrame(wxDC & dc)
{
	dc.SetBackground(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	dc.Clear();
}

void WxBugReportFrame::OnHTMLSetContent(wxThreadEvent & evt)
{
	m_htmlWin->SetPage(evt.GetString(), "");
}

void WxBugReportFrame::OnHTMLLoadPage(wxThreadEvent & evt)
{
	m_htmlWin->LoadURL(evt.GetString());
}

void WxBugReportFrame::OnClose(wxCloseEvent & evt)
{
	Destroy();
}

void WxBugReportFrame::HTMLSetContent(wxString html)
{
	wxThreadEvent* HTMLSetContentEvent = new wxThreadEvent(wxEVT_COMMAND_HTML_SET_CONTENT);

	wxString htmlContent;
	htmlContent += html;

	HTMLSetContentEvent->SetString(htmlContent);
	wxQueueEvent(wxTheApp->GetTopWindow()->GetEventHandler(), HTMLSetContentEvent);
}

void WxBugReportFrame::HTMLLoadPage(wxString url)
{
	wxThreadEvent* HTMLSetContentEvent = new wxThreadEvent(wxEVT_COMMAND_HTML_LOAD_PAGE);
	HTMLSetContentEvent->SetString(url);
	wxQueueEvent(wxTheApp->GetTopWindow()->GetEventHandler(), HTMLSetContentEvent);
}
