#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include <SDKDDKVer.h>

#define CT_VER_1    1
#define CT_VER_2    0
#define CT_VER_3    3
#define CT_VER_4    5

#define _CT_QT(X) #X
#define CT_QT(X) _CT_QT(X)

#define _CT_UT(X) L##X
#define CT_UT(X) _CT_UT(X)

#define CT_VERSION    CT_QT(CT_VER_1.CT_VER_2.CT_VER_3.CT_VER_4)  // "2.0.2.3"
#define CT_VERSION_L CT_UT(CT_VERSION)

#define CT_PROGRAM_NAME    "CoffeeTimeNow"
#define CT_PROGRAM_NAME_L CT_UT(CT_PROGRAM_NAME)
