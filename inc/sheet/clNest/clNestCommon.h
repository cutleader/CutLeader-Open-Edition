#pragma once

#include "cutLeaderCommon.h"

#if defined(ClNest_Home)
#    define ClNest_Export _declspec(dllexport)
#else
#    define ClNest_Export _declspec(dllimport)
#endif
