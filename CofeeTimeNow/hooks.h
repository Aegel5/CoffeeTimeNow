#pragma once

struct HookGlobalHandles
{
	CAutoHHOOK hHookKeyGlobal;
	CAutoHHOOK hHookMouseGlobal;
};

TStatus HookGlobal(HookGlobalHandles& handles);