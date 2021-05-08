#include "wx/wxprec.h"

#include <wx/stdpaths.h>
#include "wxBugReport.h"
#include "WxBugReportFrame.h"

#ifdef _WIN32
#include <wx\msw\registry.h>
#endif

wxIMPLEMENT_APP(WxBugReportApp);

bool WxBugReportApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;

	wxInitAllImageHandlers();

#ifdef _WIN32
	//////////////////////////////////////////////////////////////////////////
	// Make wxWebView use the latest installed Internet Explorer/Edge version
	UseLatestIEVersion();
#endif

	WxBugReportFrame* f = new WxBugReportFrame(nullptr);

	f->HTMLSetContent("<h1>Test</h1><p><a href=\"https://www.github.com\">Link test</a></p>");

	f->Show(true);

	return true;
}

#ifdef _WIN32
void WxBugReportApp::UseLatestIEVersion()
{
	enum E_InternetExplorer_RegKey
	{
		IE_7 = 7000,
		IE_8 = 8888,
		IE_9 = 9999,
		IE_10 = 10001,
		IE_EDGE = 11001
	};

	// Get the application Executable Name
	wxString ExecPath = ::wxStandardPaths::Get().GetExecutablePath();
	wxString ExecName = ExecPath.AfterLast('\\');

	// Get the installed version of Internet Explorer
	wxRegKey IEVersionKey(wxRegKey::HKLM, wxT("SOFTWARE\\Microsoft\\Internet Explorer"));

	wxString IEFullVersion;
	if (IEVersionKey.HasValue(wxT("svcVersion")))
	{
		IEVersionKey.QueryRawValue(wxT("svcVersion"), IEFullVersion);
	}
	else if (IEVersionKey.HasValue(wxT("Version")))
	{
		IEVersionKey.QueryRawValue(wxT("Version"), IEFullVersion);
	}
	else
	{
		// TODO: Handle this appropriately!
		IEVersionKey = "7.00.6001.1800";
	}

	int IEVersion = wxAtoi(IEFullVersion.BeforeFirst('.'));

	wxLogNull nolog;   // suppress error messages
	wxRegKey fbe(wxRegKey::HKCU, wxT("Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION"));

	// List here: https://support.microsoft.com/en-us/help/969393/information-about-internet-explorer-versions
	long TargetValue;
	switch (IEVersion)
	{
	case 11:
		TargetValue = IE_EDGE;
		break;
	case 10:
		TargetValue = IE_10;
		break;
	case 9:
		TargetValue = IE_9;
		break;
	case 8:
		TargetValue = IE_8;
		break;
	case 7:
	default:
		TargetValue = IE_7;
		break;
	}

	fbe.SetValue(ExecName, TargetValue);
}
#endif
