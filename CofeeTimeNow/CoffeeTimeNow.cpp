#include "stdafx.h"

TStatus MainInt(HINSTANCE hInst, LPTSTR lpCmdLine)
{
	gdata().hInst = hInst;
	IFS_LOG(gsets().Load());

	LOG_INFO_1(
		L"Start %s. Elevated=%d. Version=%s",
		lpCmdLine,
		Utils::IsSelfElevated(),
		CT_VERSION_L);

	std::vector<std::wstring> strs;
	IFS_RET(Str_Utils::Split(lpCmdLine, strs, L' '));


	if (!strs.empty() && !strs[0].empty())
	{
		std::wstring& cmd = strs[0];

		if (cmd == c_sArgMinimized)
		{
			IFS_LOG(ShowMainMenu(false));
			RETURN_SUCCESS;
		}
	}

	IFS_LOG(ShowMainMenu(true));

	RETURN_SUCCESS;
}

// Entry point
int APIENTRY wWinMain(HINSTANCE hInstance,
	HINSTANCE /*hPrevInstance*/,
	LPTSTR    lpCmdLine,
	int       nCmdShow) 
{
	auto res = MainInt(hInstance, lpCmdLine);
	IFS_LOG(res);
	return res;

}
