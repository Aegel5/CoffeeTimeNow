#pragma once


class TimeAalizer
{
	using TTime = TUInt64;
public:
	void AnalizeCurrent();
	void Init();
	void StartBreak();
	void ScheduleNewBreak();

private:

	void PrintToGui();
	TTime GetLastUserAction();
	bool IsSkipTopWnd(tstring& sExe);
	bool NeedResetTimerByIdle(TTime idleTotal);

	enum State
	{
		State_Work,
		State_Break,
		State_Idle,
	};

	void ChangeState(State state);
	State m_state = State_Work;

	TTime m_startWaitBreak = 0;
	TTime m_lastAnalized = 0;
	TTime m_breakWaitedTotal = 0;
	TTime m_idleStartTime = 0;
	TTime m_lastHiberTime = 0;

	TTime IdleTimeAfterMs()
	{
		return  gsets().idleTimeAfterSec * 1000;
	}

	TTime WorkNeedDurationMs()
	{
		return gsets().breakEverySec * 1000;
	}

	TTime BreakRemaindTime() 
	{	
		return CtUtils::SafeDec(WorkNeedDurationMs(), m_breakWaitedTotal);
	}



	bool m_pause = false;
	tstring m_skipExe;

	tstring m_lastTimeStr;

};

extern TimeAalizer* g_tcore;

