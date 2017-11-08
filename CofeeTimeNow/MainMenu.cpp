#include "stdafx.h"
#include "gui.h"
#include "hooks.h"

GuiData g_dlgData;

void ShowGui()
{
	ShowWindow(g_dlgData.hwndMainMenu, SW_SHOW);
	SetForegroundWindow(g_dlgData.hwndMainMenu);
}

void ExitMainMenu()
{
	CleanupLockWnd();
	PostQuitMessage(0);
}

TStatus HandleExitGui(HWND hwnd)
{
	LOG_INFO_1(L"Request to close gui");

	ShowWindow(g_dlgData.hwndMainMenu, SW_HIDE);

	RETURN_SUCCESS;
}
void ShowPopupMenu()
{
	HWND hwnd = g_dlgData.hwndMainMenu;

	CAutoHMENU hPopupMenu = CreatePopupMenu();

	AppendMenu(hPopupMenu, MF_STRING, ID_TRAYITEM_SHOW, L"Показать");
	AppendMenu(hPopupMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(hPopupMenu, MF_CHECKED, ID_TRAYITEM_ENABLE, L"Включить");
	AppendMenu(hPopupMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(hPopupMenu, MF_STRING, ID_TRAYITEM_EXIT, L"Выход");


	POINT curPoint;
	GetCursorPos(&curPoint);

	MENUITEMINFO inf = { 0 };
	inf.cbSize = sizeof(inf);
	inf.fState = gsets().isEnabled ? MF_CHECKED : MFS_UNCHECKED;
	inf.fMask = MIIM_STATE;
	IFW_LOG(SetMenuItemInfo(hPopupMenu, ID_TRAYITEM_ENABLE, false, &inf));

	SetForegroundWindow(hwnd);

	UINT clicked = TrackPopupMenu(
		hPopupMenu,
		TPM_RETURNCMD | TPM_NONOTIFY,
		curPoint.x,
		curPoint.y,
		0,
		hwnd,
		NULL);
	if (clicked == ID_TRAYITEM_EXIT)
	{
		ExitMainMenu();
	}
	else if (clicked == ID_TRAYITEM_SHOW)
	{
		ShowGui();
	}
	else if (clicked == ID_TRAYITEM_ENABLE)
	{
		IFS_LOG(CheckHandleEnabled(hwnd));
	}

}

TStatus InitDialogMainMenu(HWND hwnd)
{
	{
		TChar buf[512];
		wcscpy_s(buf, CT_PROGRAM_NAME_L);
		wcscat_s(buf, L" ");
		wcscat_s(buf, CT_VERSION_L);
		if (Utils::IsSelfElevated())
			wcscat_s(buf, L" Administrator");
		SetWindowText(hwnd, buf);
	}

	g_dlgData.hIcon = (HICON)LoadImage(
		gdata().hInst,
		MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON,
		GetSystemMetrics(SM_CXICON),
		GetSystemMetrics(SM_CYICON),
		0);
	g_dlgData.hIconBig = (HICON)LoadImage(
		gdata().hInst,
		MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON,
		256,
		256,
		0);

	if (g_dlgData.hIcon)
	{
		SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)g_dlgData.hIcon.get());
	}
	if (g_dlgData.hIconBig)
	{
		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)g_dlgData.hIconBig.get());
	}

	g_dlgData.trayIcon.Init(hwnd, g_dlgData.hIcon, ShowPopupMenu);
	g_dlgData.trayIcon.SetEnabled(true);

	RETURN_SUCCESS;
}



VOID CALLBACK TimerProcAnalizeTime(
	_In_ HWND     hwnd,
	_In_ UINT     uMsg,
	_In_ UINT_PTR idEvent,
	_In_ DWORD    dwTime
)
{
	g_tcore->AnalizeCurrent();
}

TStatus StartCycle(int& retFromWnd, bool fShowWnd)
{
	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = DefWindowProc;
	wcex.hInstance = gdata().hInst;
	wcex.lpszClassName = c_ctClassNameGUI;

	IFW_RET(RegisterClassEx(&wcex) != 0);

	HWND hWndFix = CreateWindow(
		c_ctClassNameGUI,
		L"Title",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0, CW_USEDEFAULT, 0, NULL, NULL, gdata().hInst, NULL);

	IFW_RET(hWndFix != NULL);
	g_dlgData.hWndFix = hWndFix;

	HWND hWnd = CreateDialog(
		gdata().hInst,
		MAKEINTRESOURCE(IDD_DIALOG_SMALL),
		NULL,
		(DLGPROC)DlgProcPageMain);

	IFW_RET(hWnd != NULL);
	g_dlgData.hwndMainMenu = hWnd;

	g_dlgData.hwndTimer = GetDlgItem(g_dlgData.hwndMainMenu, IDC_STATIC_TIME_REM);

	if (WinApiInt::ChangeWindowMessageFilterEx)
	{
		IFW_LOG(WinApiInt::ChangeWindowMessageFilterEx(g_dlgData.hWndFix, c_MSG_Quit, MSGFLT_ALLOW, 0));
		IFW_LOG(WinApiInt::ChangeWindowMessageFilterEx(g_dlgData.hWndFix, c_MSG_SHOW, MSGFLT_ALLOW, 0));
	}

	auto timeId = SetTimer(hWnd, c_timerIdAnalazeTime, c_timeAnalizeInterval, TimerProcAnalizeTime);
	IFW_RET(timeId != 0);

	g_tcore->Init();

	ShowWindow(hWnd, fShowWnd ? SW_SHOW : SW_HIDE);
	UpdateWindow(hWnd);

	//HookGlobalHandles hookHandles;
	//{
	//	IFS_RET(HookGlobal(hookHandles));
	//}

	retFromWnd = 0;
	MSG msg;
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (msg.message == WM_SYSKEYDOWN)
		{
			int k = 0;
		}

		if (bRet == -1)
		{
			// handle the error and possibly exit
			IFW_RET(FALSE);
		}

		if (msg.message == c_MSG_Quit)
		{
			LOG_INFO_1(L"Quit message");
			ExitMainMenu();
		}
		else if (msg.message == c_MSG_SHOW)
		{
			LOG_INFO_1(L"Show msg");
			ShowWindow(g_dlgData.hwndMainMenu, SW_SHOW);
		}
		else if (msg.message == c_MSG_ShowLockWnd)
		{
			IFS_LOG(ShowLockWnd());
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}

	retFromWnd = (int)msg.wParam;

	RETURN_SUCCESS;
}

TStatus ShowMainMenu(bool fShowWnd)
{
	HWND hwndGui = FindWindow(c_ctClassNameGUI, NULL);
	if (hwndGui != NULL)
	{
		LOG_INFO_1(L"Found gui=0x%X", hwndGui);
		if (fShowWnd)
		{
			LOG_INFO_1(L"Try activate");
			DWORD lpdwProcessId = 0;
			GetWindowThreadProcessId(hwndGui, &lpdwProcessId);
			PostMessage(hwndGui, c_MSG_SHOW, 0, 0);
		}
		RETURN_SUCCESS;
	}
	if (Utils::ProcSingleton(c_ctMtxSingltonGui))
	{
		LOG_INFO_1(L"Gui already running.Exit");
		RETURN_SUCCESS;
	}

	TimeAalizer tcore;
	g_tcore = &tcore;

	int retWnd = 0;
	TStatus stat = StartCycle(retWnd, fShowWnd);

	LOG_INFO_1(L"StartCycle ends");

	IFS_RET(stat);


	RETURN_SUCCESS;
}
