#pragma once

#include "cutLeaderCommon.h"

#if defined(ClSheetUI_Home)
#    define ClSheetUI_Export _declspec(dllexport)
#else
#    define ClSheetUI_Export _declspec(dllimport)
#endif