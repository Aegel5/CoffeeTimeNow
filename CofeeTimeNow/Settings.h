#pragma once

enum LockWndAnimateType
{
	LWAnimate_Null = 0,
	LWAnimate_Base = 1,
	LWAnimate_TopToBottom = 2,
};

struct Settings
{
	bool isEnabled = true;

	int breakEverySec = 25 * 60;
	int breakDurationSec = 30;
	int addToAutoStart = 0;
	int autoCloseLockWnd = 1;
	int lockWndSize = 0;
	int idleTimeAfterSec = 60 * 2;
	int minIdleRestartWaitMs = 5 * 60 * 1000;
	int lockWndAnimateTimeMs = 700;
	int closeLockWndByRightMouseBtn = 1;
	int lockWndAnimateType = LWAnimate_Base;

	int lockWndFontSize = 42;
	tstring lockWndFontFamily = L"Tahoma";
	int lockWndFontColor = 0xFFFFFF;
	int lockWndFontWeight = 0;

	int lockWndBackgroundColor = 0x000000;

	std::vector<std::wstring> disableInPrograms;

	static Settings& Global()
	{
		static Settings settings;
		return settings;
	}

	TStatus LoadLua();

	TStatus Load()
	{
		IFS_RET(LoadLua());

		RETURN_SUCCESS;
	}
};

inline Settings& gsets() { return Settings::Global(); }