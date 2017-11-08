#include "stdafx.h"
#include "gui.h"
#include "TimeAnalizer.h"
#include "startup/reguser.h"
#include "utils/procstart.h"

TStatus HandleStart1(HWND hwnd)
{
	g_tcore->StartBreak();
	RETURN_SUCCESS;
}

TStatus EditBinFile(TStr fileName)
{
	tstring sFilePath;
	IFS_RET(CtUtils::GetPathBinFile(sFilePath, fileName));

	procstart::CreateProcessParm parm;
	parm.sExe = L"notepad.exe";
	parm.sCmd = sFilePath.c_str();
	parm.mode = procstart::SW_CREATEPROC_SHELLEXE;
	CAutoHandle hProc;
	IFS_RET(procstart::SwCreateProcess(parm, hProc));

	RETURN_SUCCESS;
}

TStatus HandleHelp(HWND hwnd)
{
	IFS_RET(EditBinFile(L"readme.txt"));
	RETURN_SUCCESS;
}

TStatus HandleSettings(HWND hwnd)
{
	IFS_RET(EditBinFile(L"config.lua"));
	RETURN_SUCCESS;
}



TStatus UpdateEnabled(HWND hwnd)
{
	gui_tools::SetCheckBox(hwnd, IDC_CHECK_ENABLED, gsets().isEnabled);
	RETURN_SUCCESS;
}

TStatus CheckHandleEnabled(HWND hwnd)
{
	gsets().isEnabled = !gsets().isEnabled;
	IFS_RET(UpdateEnabled(hwnd));
	RETURN_SUCCESS;
}

TStatus UpdateAutoStart(HWND hwnd)
{
	std::wstring sPath;
	IFS_RET(GetPath(sPath, PATH_TYPE_EXE_PATH, SW_BIT_32));

	bool isAllOk;
	bool isHasTask;

	IFS_RET(Startup::CheckAutoStartUser(isAllOk, isHasTask, c_sRegRunValue, sPath.c_str(), c_sArgMinimized));

	if (gsets().addToAutoStart)
	{
		if (!isAllOk)
		{
			IFS_RET(Startup::AddWindowsRun(c_sRegRunValue, sPath.c_str(), c_sArgMinimized));
		}
	}
	else
	{
		if (isHasTask)
		{
			IFS_RET(Startup::RemoveWindowsRun(c_sRegRunValue));
		}
	}

	gui_tools::SetCheckBox(hwnd, IDC_CHECK_AUTO, gsets().addToAutoStart);

	RETURN_SUCCESS;
}

TStatus HandleApply(HWND hwnd)
{
	IFS_RET(gsets().Load());
	IFS_RET(UpdateAutoStart(hwnd));
	RETURN_SUCCESS;
}

TStatus CheckHandleAutostart(HWND hwnd)
{
	gsets().addToAutoStart = !gsets().addToAutoStart;
	IFS_RET(UpdateAutoStart(hwnd));
	RETURN_SUCCESS;
}

LRESULT CALLBACK DlgProcPageMain(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	gui_tools::Wmsg wmsg(hwnd, msg, wParam, lParam);

	if (g_dlgData.trayIcon.ProcMsg(wmsg))
	{
		return TRUE;
	}

	if (gui_tools::HandleButton(wmsg, IDC_BUTTON_CLOSE, HandleExitGui))
		return true;

	if (wmsg.msg == WM_CLOSE)
	{
		HandleExitGui(wmsg.hwnd);
		return true;
	}

	if (gui_tools::HandleButton(wmsg, IDC_BUTTON_START_1, HandleStart1))
		return TRUE;

	if (gui_tools::HandleButton(wmsg, IDC_BUTTON_HELP, HandleHelp))
		return TRUE;

	if (gui_tools::HandleButton(wmsg, IDC_BUTTON_SETS, HandleSettings))
		return TRUE;

	if (gui_tools::HandleButton(wmsg, IDC_BUTTON_APPLY, HandleApply))
		return TRUE;

	if (gui_tools::HandleButton(wmsg, IDC_CHECK_ENABLED, CheckHandleEnabled))
		return TRUE;

	if (gui_tools::HandleButton(wmsg, IDC_CHECK_AUTO, CheckHandleAutostart))
		return TRUE;

	if (msg == WM_INITDIALOG)
	{
		InitDialogMainMenu(wmsg.hwnd);
		IFS_LOG(UpdateEnabled(wmsg.hwnd));
		IFS_LOG(UpdateAutoStart(hwnd));
		return TRUE;
	}

	if (msg == WM_RBUTTONUP)
	{
		ShowPopupMenu();
		return TRUE;
	}

	return FALSE;
}