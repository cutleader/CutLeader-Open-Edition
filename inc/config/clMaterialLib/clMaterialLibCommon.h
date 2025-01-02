#pragma once

#include "cutLeaderCommon.h"

#if defined(ClMaterialLib_Home)
#    define ClMaterialLib_Export _declspec(dllexport)
#else
#    define ClMaterialLib_Export _declspec(dllimport)
#endif