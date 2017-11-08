#include "stdafx.h"

#include "Settings.h"
#include "luawrap/luawrap.h"


TStatus Settings::LoadLua()
{
	tstring sFilePath;
	IFS_RET(CtUtils::GetPathBinFile(sFilePath, L"config.lua"));

	CAutoLuaState luaState = luaL_newstate();
	if (!luaState)
	{
		RETS(SW_ERR_LUA_ERROR);
	}

	std::string sFilePathA;
	IFS_RET(Str_Utils::WideToUtf8(sFilePath.c_str(), sFilePathA));
	int loadres = luaL_dofile(luaState, sFilePathA.c_str());
	if (loadres != 0)
	{
		TChar sMsg[512];
		swprintf_s(sMsg, L"Error load config file: %S", lua_tostring(luaState, -1));
		MessageBox(NULL, sMsg, L"Lua error", 0);
		LUARET(loadres, L"%s", sMsg);
	}

	int logLevel;
	if (LuaGetInt(luaState, "ll", logLevel))
	{
		SetLogLevel((TLogLevel)logLevel);
		LOG_INFO_1(L"Set ll %u", logLevel);
	}

	LuaGetInt(luaState, "breakEverySec", breakEverySec);
	LuaGetInt(luaState, "breakDurationSec", breakDurationSec);
	LuaGetInt(luaState, "addToAutoStart", addToAutoStart);
	LuaGetInt(luaState, "autoCloseLockWnd", autoCloseLockWnd);
	LuaGetInt(luaState, "lockWndSize", lockWndSize);
	LuaGetInt(luaState, "idleTimeAfterSec", idleTimeAfterSec);
	LuaGetInt(luaState, "minIdleRestartWaitMs", minIdleRestartWaitMs);
	LuaGetInt(luaState, "lockWndAnimateTimeMs", lockWndAnimateTimeMs);
	LuaGetInt(luaState, "closeLockWndByRightMouseBtn", closeLockWndByRightMouseBtn);
	LuaGetInt(luaState, "lockWndAnimateType", lockWndAnimateType);

	LuaGetString(luaState, "lockWndFontFamily", lockWndFontFamily);
	LuaGetInt(luaState, "lockWndFontSize", lockWndFontSize);
	LuaGetInt(luaState, "lockWndFontWeight", lockWndFontWeight);

	LuaGetInt(luaState, "lockWndBackgroundColor", lockWndBackgroundColor);
	LuaGetInt(luaState, "lockWndFontColor", lockWndFontColor);

	LuaLoadLowStringList(luaState, disableInPrograms, "disableInPrograms");

	RETURN_SUCCESS;
}
