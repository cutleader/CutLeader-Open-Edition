#pragma once

#include "cutLeaderCommon.h"

#if defined(ClReportPrinter_Home)
#    define ClReportPrinter_Export _declspec(dllexport)
#else
#    define ClReportPrinter_Export _declspec(dllimport)
#endif