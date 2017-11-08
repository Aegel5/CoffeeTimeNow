#pragma once

static const DWORD c_timerAnimateId = 9;
static const TChar c_ctClassNameGUI[] = CT_PROGRAM_NAME_L "_A4EA837F";
static const TChar c_ctClassNameLockWnd[] = CT_PROGRAM_NAME_L "_C1CA44FE";
static const TChar c_ctClassNameNotifyWnd[] = CT_PROGRAM_NAME_L L"_7BFC95B1";
static const TChar c_ctMtxSingltonGui[] = L"Local\\" CT_PROGRAM_NAME_L  L"_024AA0B1";

static const TChar c_sArgMinimized[] = L"/minimized";

const static TChar c_sRegRunValue[] = CT_PROGRAM_NAME_L L"_AutoStart";

static const UINT c_MSG_SHOW = 0xBFFF - 33;
static const UINT c_MSG_Quit = 0xBFFF - 35;
static const UINT c_MSG_ShowLockWnd = 0xBFFF - 34;

const static int c_nTrayIconId = 6332;
const static int WM_TRAYICON = WM_USER + 100;
const static int ID_TRAYITEM_EXIT = 3000;
const static int ID_TRAYITEM_SHOW = 3001;
const static int ID_TRAYITEM_ENABLE = 3002;
const static int ID_TRAYITEM_ABOUT = 3003;

const static UINT c_timeAnalizeInterval = 200;

static const UINT c_timerIdAddToTray = 8;
static const UINT c_timerIdAnalazeTime = 7;
static const UINT c_timerUpdLockTime = 6;
static const UINT c_timerIdAnimateLockWnd = 15;
static const UINT c_timerIdAnimateNotifyWnd = 16;
//static const UINT c_timerAnimateWnd = 17;
