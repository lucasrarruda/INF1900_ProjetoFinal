
// View.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include <Resource.h>		// main symbols

// CViewApp:
// See View.cpp for the implementation of this class
//

class CViewApp : public CWinApp
{
public:
	CViewApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

};

extern CViewApp theApp;
