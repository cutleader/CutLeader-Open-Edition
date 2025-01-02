#pragma once

#include "cutLeaderCommon.h"

#if defined(ClReportTemplateUI_Home)
#    define ClReportTemplateUI_Export _declspec(dllexport)
#else
#    define ClReportTemplateUI_Export _declspec(dllimport)
#endif

