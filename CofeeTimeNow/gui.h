#pragma once

#include "Shellapi.h"
#include "../libtools/gui/inc_all.h"

struct GuiData
{
	HWND hWndFix = nullptr;
	HWND hwndMainMenu = nullptr;
	HWND hwndTimer = nullptr;

	//HWND hwndPageMain;
	//gui_tools::SimpleTabs smTabs;

	gui_tools::TrayIcon<WM_TRAYICON, c_timerIdAddToTray, c_nTrayIconId> trayIcon;

	CAutoHandleIcon hIcon;
	CAutoHandleIcon hIconBig;
};

extern GuiData g_dlgData;


TStatus ShowMainMenu(bool fShowWnd);
void ShowPopupMenu();
LRESULT CALLBACK DlgProcPageMain(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
TStatus CheckHandleEnabled(HWND hwnd);


TStatus ShowLockWnd();
void CleanupLockWnd();

inline void ReqestLockWnd()
{
	PostMessage(g_dlgData.hWndFix, c_MSG_ShowLockWnd, 0, 0);
}

inline TStatus RequestBreak()
{
	ReqestLockWnd();
	RETURN_SUCCESS;
}

TStatus HandleExitGui(HWND hwnd);
TStatus InitDialogMainMenu(HWND hwnd);




