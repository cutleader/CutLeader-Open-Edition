#pragma once

#include "cutLeaderCommon.h"

#if defined(ClDataImportExportUI_Home)
#    define ClDataImportExportUI_Export _declspec(dllexport)
#else
#    define ClDataImportExportUI_Export _declspec(dllimport)
#endif

