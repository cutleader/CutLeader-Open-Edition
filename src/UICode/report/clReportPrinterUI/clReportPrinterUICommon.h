#pragma once

#include "cutLeaderCommon.h"

#if defined(ClReportPrinterUI_Home)
#    define ClReportPrinterUI_Export _declspec(dllexport)
#else
#    define ClReportPrinterUI_Export _declspec(dllimport)
#endif