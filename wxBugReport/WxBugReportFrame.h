#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/webview.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/gbsizer.h>
#include <wx/textctrl.h>
#include <wx/gauge.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/thread.h>
#include <wx/event.h>

wxDECLARE_EVENT(wxEVT_COMMAND_HTML_SET_CONTENT, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_COMMAND_HTML_LOAD_PAGE, wxThreadEvent);

class WxBugReportFrame : public wxFrame
{
private:

protected:
	wxWebView* m_htmlWin;
	wxButton* m_btnClose;

public:
	WxBugReportFrame(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Bug report sample app"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800, 600), long style = 0L /* wxCAPTION|wxCLOSE_BOX|wxSYSTEM_MENU */);
	~WxBugReportFrame();

protected:
	wxString m_LaunchExecutableName;

public:
	//////////////////////////////////////////////////////////////////////////
	// Close button click
	void OnCloseButtonClicked(wxCommandEvent& WXUNUSED(evt));

	//////////////////////////////////////////////////////////////////////////
	// wxWebView Navigating Event (i.e. When clicking on links)
	void OnClickLink(wxWebViewEvent& evt);

	//////////////////////////////////////////////////////////////////////////
	// Background image
//	void PaintEvent(wxPaintEvent & evt);
	void RenderFrame(wxDC& dc);

	//////////////////////////////////////////////////////////////////////////
	// Thread communication
	void OnHTMLSetContent(wxThreadEvent& evt);
	void OnHTMLLoadPage(wxThreadEvent& evt);

	void OnClose(wxCloseEvent& evt);

	static void HTMLSetContent(wxString html);
	static void HTMLLoadPage(wxString url);

protected:
	wxCriticalSection		m_pThreadCS;				// protects the m_pThread pointer

};
