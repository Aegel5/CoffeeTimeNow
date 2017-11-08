#pragma once

struct GlobalData
{
	HINSTANCE hInst;
	static GlobalData& Global()
	{
		static GlobalData data;
		return data;
	}
};

inline GlobalData& gdata() { return GlobalData::Global(); }
