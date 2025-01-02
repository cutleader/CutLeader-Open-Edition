#pragma once

#include "cutLeaderCommon.h"

#if defined(ClReportConfig_Home)
#    define ClReportConfig_Export _declspec(dllexport)
#else
#    define ClReportConfig_Export _declspec(dllimport)
#endif

typedef enum tagTplUnitType
{
	TPL_UNIT_PIXEL			= 0,
	TPL_UNIT_INCH			= 1,
	TPL_UNIT_CM				= 2,
} TPL_UNIT_TYPE;

typedef enum tagTextJustType
{
	TEXT_JUST_LEFT				= 0,
	TEXT_JUST_CENTER			= 1,
	TEXT_JUST_RIGHT				= 2,
} TEXT_JUST_TYPE;