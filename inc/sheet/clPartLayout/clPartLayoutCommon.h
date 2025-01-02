#pragma once

#include "cutLeaderCommon.h"

#if defined(ClPartLayout_Home)
#    define ClPartLayout_Export _declspec(dllexport)
#else
#    define ClPartLayout_Export _declspec(dllimport)
#endif

