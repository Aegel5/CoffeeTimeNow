#pragma once

class AnimateWnd
{
private:
	BYTE m_min = 0;
	BYTE m_max = 0;
	int m_curAlpha = 0;
	TUInt64 m_totaltime = 0;
	TUInt64 m_startTime = 0;
	TUInt64 m_endTime = 0;

	void SetAlpha(HWND hwnd, BYTE val)
	{
		IFW_LOG(::SetLayeredWindowAttributes(hwnd, RGB(1, 1, 1), val, LWA_ALPHA));
	}
public:

	void SetWndParm(HWND hwnd)
	{
		SetAlpha(hwnd, m_curAlpha);
	}

	bool CalcParm()
	{
		TUInt64 timeCur = GetTickCount64();
		TUInt64 deltTime = timeCur - m_startTime;
		int totalAlp = m_max - m_min;

		double curAlphaD = double(totalAlp) * deltTime / m_totaltime;
		int curA = (int)curAlphaD;
		if (curA > m_max)
		{
			curA = m_max;
		}

		bool fChanged = curA != m_curAlpha;

		m_curAlpha = curA;

		return fChanged;
	}

	bool Finished()
	{
		return m_curAlpha == m_max;
	}

	TStatus Init(int time, BYTE min, BYTE max)
	{
		m_startTime = GetTickCount64();
		m_endTime = m_startTime + time;
		m_totaltime = time;
		m_min = min;
		m_max = max;

		RETURN_SUCCESS;
	}
};