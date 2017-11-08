#pragma once

namespace CtUtils
{

	struct TimeDispl
	{
		int hours;
		int min;
		int sec;
		int days;
		int ms;

		void FromMs(TUInt64 valMs)
		{
			auto x = valMs;

			ms = x % 1000;
			x /= 1000;

			sec = x % 60;
			x /= 60;

			min = x % 60;
			x /= 60;

			hours = x % 24;
			x /= 24;

			days = (int)x;
		}
		void tostring(tstring& str)
		{
			TChar sOut[256];
			swprintf_s(sOut, L"%02u:%02u:%02u", hours, min, sec);
			str = sOut;
		}
		void tostringMs(tstring& str)
		{
			TChar sOut[256];
			swprintf_s(sOut, L"%02u:%02u:%02u.%02u", hours, min, sec, ms / 10);
			str = sOut;
		}
	};

	inline TUInt64 SafeDec(TUInt64 val, TUInt64 d)
	{
		if (val < d)
		{
			return 0;
		}
		else
		{
			return val - d;
		}
	}



	inline TStatus GetPathBinFile(tstring& sFilePath, TStr fileName)
	{
		IFS_RET(GetPath(sFilePath, PATH_TYPE_SELF_FOLDER, GetSelfBit()));
		sFilePath += fileName;

		if (!FileUtils::IsFileExists(sFilePath.c_str()))
		{
			return SW_ERR_SUCCESS;
		}

		RETURN_SUCCESS;
	}
}


