#include "stdafx.h"
#include "TimeAnalizer.h"
#include "gui.h"

TimeAalizer* g_tcore = NULL;

void TimeAalizer::AnalizeCurrent()
{
	TTime curTime = GetTickCount64();
	TTime curTimeSegment = curTime - m_lastAnalized;

	if (curTimeSegment >= c_timeAnalizeInterval * 3)
	{
		LOG_INFO_1(L"Cut time segment from=%I64u to=%Iu. Possible hibernating.", curTimeSegment, c_timeAnalizeInterval);
		if (NeedResetTimerByIdle(curTimeSegment))
		{
			ScheduleNewBreak();
			LOG_INFO_1(L"Restart waiting break because too much hibernate=%I64u", curTimeSegment);
		}

		curTimeSegment = c_timeAnalizeInterval;
		m_lastHiberTime = GetTickCount64();

	}

	TTime timeIdleSegment = curTime - GetLastUserAction();

	m_pause = !gsets().isEnabled;

	m_skipExe.clear();
	if (!m_pause && !gsets().disableInPrograms.empty())
	{
		m_pause = IsSkipTopWnd(m_skipExe);
	}
	LOG_INFO_4(L"analize=%I64u, m_pause=%u, m_skipExe=%s", curTime, m_pause, m_skipExe.c_str());

	if(m_pause)
	{
		LOG_INFO_4(L"skip pause");
	}
	else
	{
		if (m_state == State_Work)
		{
			m_breakWaitedTotal += curTimeSegment;

			if (timeIdleSegment >= IdleTimeAfterMs())
			{
				ChangeState(State_Idle);

				TTime workedSegm = timeIdleSegment / 4;
				TTime idleSegm = timeIdleSegment - workedSegm;

				m_idleStartTime = curTime - idleSegm;

				m_breakWaitedTotal = CtUtils::SafeDec(m_breakWaitedTotal, idleSegm);
				LOG_INFO_1(L"m_breakWaitedTotal: %I64u", m_breakWaitedTotal);

				LOG_INFO_1(L"timeIdleSegment: %I64u, idleSegm:%I64u", timeIdleSegment, idleSegm);
			}

			if (BreakRemaindTime() == 0)
			{
				StartBreak();
			}
		}
		else if (m_state == State_Idle)
		{
			TTime idleTotal = curTime - m_idleStartTime;

			if (m_breakWaitedTotal > 0 && NeedResetTimerByIdle(idleTotal))
			{
				LOG_INFO_1(L"Clear wait timer because of idle");
				m_breakWaitedTotal = 0;
			}

			if (timeIdleSegment < IdleTimeAfterMs())
			{
				if (NeedResetTimerByIdle(idleTotal))
				{
					ScheduleNewBreak();
					LOG_INFO_1(L"Restart waiting break because too much idle=%I64u", idleTotal);
				}
				else
				{
					ChangeState(State_Work);
				}
			}
		}
		else if (m_state == State_Break)
		{

		}
	}
	
	LOG_INFO_4(L"m_breakWaitedTotal: %I64u", m_breakWaitedTotal);
	m_lastAnalized = curTime;

	PrintToGui();
}
void TimeAalizer::Init()
{
	LOG_INFO_1(L"TimeAalizer::Init");

	ScheduleNewBreak();
	AnalizeCurrent();
}

void TimeAalizer::StartBreak()
{
	ChangeState(State_Break);
	RequestBreak();
}

void TimeAalizer::ScheduleNewBreak()
{
	LOG_INFO_1(L"ScheduleNewBreak");

	ChangeState(State_Work);

	m_idleStartTime = 0;
	m_startWaitBreak = GetTickCount64();
	m_lastAnalized = m_startWaitBreak;
	m_breakWaitedTotal = 0;

}

void TimeAalizer::PrintToGui()
{
	tstring sdisp;

	CtUtils::TimeDispl tdisp;
	tdisp.FromMs(BreakRemaindTime());
	tdisp.tostring(sdisp);

	if (m_pause)
	{
		if (!m_skipExe.empty())
		{
			tstring sShortName = m_skipExe;
			if (sShortName.size() > 18)
			{
				sShortName.erase(18);
				sShortName += L"...";
			}
			sdisp += L" (" + sShortName + L")";
		}
		else
		{
			sdisp += L" (pause)";
		}
	}
	else
	{
		if (m_state == State_Idle)
		{
			sdisp += L" (IDLE)";
		}
		else if (m_state == State_Break)
		{
			sdisp += L" (NOW)";
		}
	}

	if (m_lastTimeStr != sdisp)
	{
		m_lastTimeStr = sdisp;
		SetWindowText(g_dlgData.hwndTimer, m_lastTimeStr.c_str());
	}
}
TimeAalizer::TTime TimeAalizer::GetLastUserAction()
{
	LASTINPUTINFO inputInfo;
	SwZeroMemory(inputInfo);
	inputInfo.cbSize = sizeof(inputInfo);
	IFW_LOG(GetLastInputInfo(&inputInfo));

	TTime lastUserAction = GetTickCount64();
	lastUserAction &= 0xFFFFFFFF00000000;
	lastUserAction |= inputInfo.dwTime;


	if(lastUserAction < m_lastHiberTime)
	{
		lastUserAction = m_lastHiberTime;
	}

	return lastUserAction;
}

bool TimeAalizer::IsSkipTopWnd(tstring& sExe)
{
	HWND hwndFocused = nullptr;

	GUITHREADINFO gui = { 0 };
	gui.cbSize = sizeof(gui);
	GetGUIThreadInfo(0, &gui);
	hwndFocused = gui.hwndFocus;

	if (hwndFocused == nullptr)
		return false;

	DWORD dwTopPid = 0;
	DWORD dwIdThreadTopWnd = GetWindowThreadProcessId(hwndFocused, &dwTopPid);

	if (dwTopPid == 0)
		return false;

	tstring spath;
	tstring sname;
	Utils::GetProcLowerNameByPid(dwTopPid, spath, sname);

	if (sname.empty())
		return false;

	for (auto& pname : gsets().disableInPrograms)
	{
		if (pname == sname)
		{
			sExe = sname;
			return true;
		}
	}
	return false;
}

bool TimeAalizer::NeedResetTimerByIdle(TTime idleTotal)
{
	TTime breakDurationInc = gsets().breakDurationSec * 1000 * 3 / 2;
	if (idleTotal >= gsets().minIdleRestartWaitMs && idleTotal >= breakDurationInc)
	{
		return true;
	}

	return false;
}

void TimeAalizer::ChangeState(State state)
{
	m_state = state;
	LOG_INFO_1(L"Change state to %u", TUInt32(state));
}
