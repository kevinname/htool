
// hs66xx_tools.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Chs66xx_toolsApp:
// See hs66xx_tools.cpp for the implementation of this class
//

class CHToolCApp : public CWinApp
{
public:
	CHToolCApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CHToolCApp theApp;