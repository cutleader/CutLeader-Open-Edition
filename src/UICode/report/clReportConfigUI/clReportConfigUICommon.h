#pragma once

#include "cutLeaderCommon.h"

#if defined(ClReportConfigUI_Home)
#    define ClReportConfigUI_Export _declspec(dllexport)
#else
#    define ClReportConfigUI_Export _declspec(dllimport)
#endif

