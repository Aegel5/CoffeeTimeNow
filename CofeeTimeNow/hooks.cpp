#include "stdafx.h"
#include "hooks.h"
#include "TimeAnalizer.h"


LRESULT CALLBACK LowLevelKeyboardProc(
	_In_  int nCode,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
)
{
	if (nCode == HC_ACTION)
	{
	}

	return CallNextHookEx(0, nCode, wParam, lParam);

}

LRESULT CALLBACK LowLevelMouseProc(
	_In_  int nCode,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
)
{
	if (nCode == HC_ACTION)
	{
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

TStatus HookGlobal(HookGlobalHandles& handles)
{
	LOG_INFO_1(L"HookGlobal...");

	CAutoHMODULE hModUser;
	if (!IsWindowsVistaOrGreater())
	{
		hModUser = LoadLibrary(L"user32.dll");
	}
	handles.hHookKeyGlobal = WinApiInt::SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hModUser, 0);
	IFW_RET(handles.hHookKeyGlobal.IsValid());

#ifndef _DEBUG
	handles.hHookMouseGlobal = WinApiInt::SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hModUser, 0);
	IFW_RET(handles.hHookMouseGlobal.IsValid());
#endif


	RETURN_SUCCESS;
}