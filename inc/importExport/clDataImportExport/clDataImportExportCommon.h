#pragma once

#include "cutLeaderCommon.h"

#if defined(ClDataImportExport_Home)
#    define ClDataImportExport_Export _declspec(dllexport)
#else
#    define ClDataImportExport_Export _declspec(dllimport)
#endif

typedef enum tagImpExpDataType
{
	IMPEXP_DATA_NOT_INIT	= 0,

	IMPEXP_DATA_DXF			= 1,
	IMPEXP_DATA_DWG			= 2,
	IMPEXP_DATA_TAOPRT		= 3,
	IMPEXP_DATA_TAOSHT		= 4,

	IMPEXP_DATA_TAOEXPLIB	= 10,

	IMPEXP_DATA_BK_DB		= 15,
} IMPEXP_DATA_TYPE;