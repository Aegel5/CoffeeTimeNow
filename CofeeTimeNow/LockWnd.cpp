#include "stdafx.h"
#include "AnimateWnd.h"
#include "gui.h"
#include "gui/img_res.h"
#include "utils/str_utils.h"
#include "Strsafe.h"

LRESULT CALLBACK WndProcLock(HWND, UINT, WPARAM, LPARAM);

struct WndMon
{
	RECT rc;
	HWND hwnd = nullptr;
};

struct DynParm2
{
	AnimateWnd animate;
	CAutoHFONT hFont;
	CAutoCloseHBRUSH hBrushBg;

	ULONGLONG timeLockTotal = 0;
	ULONGLONG timeLockCur = 0;
	ULONGLONG dwTimeStart = 0;
	tstring curStrTime;

	std::vector<WndMon> wndMons;
};

static DynParm2* g_dyn = nullptr;

void CloseLockWnd()
{
	CleanupLockWnd();
	g_tcore->ScheduleNewBreak();
}

void CleanupLockWnd()
{
	KillTimer(g_dlgData.hwndMainMenu, c_timerUpdLockTime);

	if (!g_dyn)
		return;

	for (auto& wm : g_dyn->wndMons)
	{
		DestroyWindow(wm.hwnd);
	}

	delete g_dyn;
	g_dyn = nullptr;
}

void TimeFunc()
{
	bool fNeedKill = true;

	if(gsets().lockWndAnimateType == LWAnimate_Base)
	{
		if (g_dyn->animate.CalcParm())
		{
			for (auto& wm : g_dyn->wndMons)
			{
				LOG_INFO_4(L"Change alpha parms");
				g_dyn->animate.SetWndParm(wm.hwnd);
			}
		}

		if (!g_dyn->animate.Finished())
		{
			fNeedKill = false;
		}
	}

	bool fNeedRePaint = false;

	// timer
	{
		ULONGLONG delt = GetTickCount64() - g_dyn->dwTimeStart;
		bool timeOut = delt >= g_dyn->timeLockTotal;
		if (timeOut)
		{
			delt = g_dyn->timeLockTotal;
		}
		else
		{
			fNeedKill = false;
		}

		g_dyn->timeLockCur = g_dyn->timeLockTotal - delt;

		CtUtils::TimeDispl tdisp;
		tdisp.FromMs(g_dyn->timeLockCur);
		tstring sdisp;
		tdisp.tostring(sdisp);

		if (g_dyn->curStrTime != sdisp)
		{
			g_dyn->curStrTime = sdisp;
			fNeedRePaint = true;
		}

	}

	if (fNeedKill)
	{
		KillTimer(g_dlgData.hwndMainMenu, c_timerUpdLockTime);
		if (gsets().autoCloseLockWnd)
		{
			CloseLockWnd();
		}
	}
	else if (fNeedRePaint)
	{
		LOG_INFO_4(L"need repaint");
		for (auto& wm : g_dyn->wndMons)
		{
			InvalidateRect(wm.hwnd, NULL, NULL);
			//UpdateWindow(wm.hwnd);
		}
	}
}


VOID CALLBACK TimeProcUpdTimeLock(
	_In_ HWND     hwnd,
	_In_ UINT     uMsg,
	_In_ UINT_PTR idEvent,
	_In_ DWORD    dwTime
)
{
	TimeFunc();
}

BOOL CALLBACK MonitorEnumProc(
	_In_ HMONITOR hMonitor,
	_In_ HDC      hdcMonitor,
	_In_ LPRECT   lprcMonitor,
	_In_ LPARAM   dwData
)
{
	MONITORINFOEX moninfo;
	SwZeroMemory(moninfo);
	moninfo.cbSize = sizeof(moninfo);
	GetMonitorInfo(hMonitor, &moninfo);

	g_dyn->wndMons.push_back(WndMon());
	g_dyn->wndMons.back().rc = *lprcMonitor;

	return TRUE;
}

// Entry point
TStatus ShowLockWnd()
{
	CleanupLockWnd();

	g_dyn = new DynParm2();
	if (g_dyn == nullptr)
		RETS(SW_ERR_NO_MEMORY);

	{
		WNDCLASSEXW wcex = { 0 };
		wcex.cbSize = sizeof(wcex);
		//wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProcLock;
		wcex.hInstance = gdata().hInst;
		wcex.hCursor = ::LoadCursorW(NULL, IDC_ARROW);
		//wcex.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
		wcex.lpszClassName = c_ctClassNameLockWnd;
		::RegisterClassExW(&wcex);
	}

	IFW_RET(EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, NULL));

	{
		LOGFONT lf;
		ZeroMemory(&lf, sizeof(lf));
		lf.lfHeight = gsets().lockWndFontSize;
		StringCchCopy(lf.lfFaceName, LF_FACESIZE, gsets().lockWndFontFamily.c_str());
		lf.lfWeight = gsets().lockWndFontWeight;
		g_dyn->hFont = CreateFontIndirect(&lf);
	}

	g_dyn->hBrushBg = CreateSolidBrush(gui_tools::ColorFromRGB(gsets().lockWndBackgroundColor));

	for (auto& wm : g_dyn->wndMons)
	{
		int x = wm.rc.left;
		int y = wm.rc.top;

		auto w = wm.rc.right - x;
		auto h = wm.rc.bottom - y;

		if (gsets().lockWndSize != 0)
		{
			h = gsets().lockWndSize;
		}

		DWORD dwExStyle = WS_EX_TOPMOST;
		if (gsets().lockWndAnimateType <= LWAnimate_Base)
		{
			dwExStyle |= WS_EX_LAYERED;
		}

		DWORD dwStyle = WS_POPUP;
		if (gsets().lockWndAnimateType > LWAnimate_Base)
		{
			dwStyle |= WS_BORDER;
		}

		HWND hwnd = ::CreateWindowExW(
			dwExStyle,
			c_ctClassNameLockWnd,
			L"Test",
			dwStyle,
			x,
			y,
			w,
			h,
			NULL,
			NULL,
			gdata().hInst,
			NULL);
		IFW_RET(hwnd != nullptr);

		wm.hwnd = hwnd;

		IFW_RET(SetWindowPos(hwnd, HWND_TOP, x, y, w, h, 0));

		if (gsets().lockWndAnimateType <= LWAnimate_Base)
		{
			IFW_LOG(::SetLayeredWindowAttributes(hwnd, RGB(1, 1, 1), 255, LWA_ALPHA));
			IFW_LOG(::ShowWindow(hwnd, SW_SHOW));
		}
		else
		{
			DWORD dwFlags = AW_VER_POSITIVE;
			AnimateWindow(hwnd, gsets().lockWndAnimateTimeMs, dwFlags);
		}

		::UpdateWindow(hwnd);
	}

	IFS_RET(g_dyn->animate.Init(gsets().lockWndAnimateTimeMs, 0, 255));

	g_dyn->timeLockTotal = gsets().breakDurationSec * 1000;
	g_dyn->timeLockCur = g_dyn->timeLockTotal;
	g_dyn->dwTimeStart = GetTickCount64();

	TimeFunc();

	auto timeId = SetTimer(g_dlgData.hwndMainMenu, c_timerUpdLockTime, 10, TimeProcUpdTimeLock);
	IFW_LOG(timeId != 0);

	RETURN_SUCCESS;
}
LRESULT CALLBACK WndProcLock(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	gui_tools::Wmsg wmsg(hwnd, message, wParam, lParam);

	if (message == WM_KEYDOWN || message == WM_MOUSEMOVE)
	{
		if (g_dyn->timeLockCur == 0)
		{
			CloseLockWnd();
			return 0;
		}
	}

	if (message == WM_RBUTTONUP)
	{
		if (gsets().closeLockWndByRightMouseBtn || g_dyn->timeLockCur == 0)
		{
			CloseLockWnd();
		}
		return 0;
	}

	if(message == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		HBRUSH hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		if (g_dyn->hBrushBg)
		{
			hBrush = g_dyn->hBrushBg;
		}
		FillRect(hdc, &ps.rcPaint, hBrush);

		RECT rc;
		GetClientRect(hwnd, &rc);

		int wClient = rc.right - rc.left;
		int hClient = rc.bottom - rc.top;

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, gui_tools::ColorFromRGB(gsets().lockWndFontColor));
		SelectObject(hdc, g_dyn->hFont);

		tstring& sdisp = g_dyn->curStrTime;

		SIZE size;
		GetTextExtentPoint32(hdc, sdisp.c_str(), sdisp.size(), &size);

		int x = (wClient >> 1) - (size.cx >> 1);
		int y = (hClient >> 1) - (size.cy >> 1);

		TextOut(hdc, x, y, sdisp.c_str(), sdisp.size());

		EndPaint(hwnd, &ps);

		return TRUE;
	}

	return ::DefWindowProc(hwnd, message, wParam, lParam);
}