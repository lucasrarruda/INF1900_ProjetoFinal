
// View.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include <View.h>
#include <MainFrame.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CViewApp

BEGIN_MESSAGE_MAP(CViewApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CViewApp construction

CViewApp::CViewApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CViewApp object

CViewApp theApp;


// CViewApp initialization

BOOL CViewApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Criar uma instância da nova classe de janela
	MainFrame* pWindow = new MainFrame;
	if (!pWindow || !pWindow->Create())
	{
		AfxMessageBox(_T("Falha ao criar a janela!"));
		delete pWindow;
		return FALSE;
	}

	// Exibir e atualizar a janela
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();

	// Adicione um loop de mensagem para manter o aplicativo em execução
	m_pMainWnd = pWindow;
	m_pMainWnd->UpdateWindow();

	int nRet = Run();

	return TRUE;
}

