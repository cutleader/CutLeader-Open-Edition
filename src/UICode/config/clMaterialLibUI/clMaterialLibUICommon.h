#pragma once

#include "cutLeaderCommon.h"

#if defined(ClMaterialLibUI_Home)
#    define ClMaterialLibUI_Export _declspec(dllexport)
#else
#    define ClMaterialLibUI_Export _declspec(dllimport)
#endif