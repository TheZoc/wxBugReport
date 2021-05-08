#pragma once

#include <wx/wxprec.h>

class WxBugReportApp : public wxApp
{
public:
	virtual bool OnInit();

#ifdef _WIN32
	// Make wxWebView use the latest installed Internet Explorer/Edge version on Windows
	void UseLatestIEVersion();
#endif
};