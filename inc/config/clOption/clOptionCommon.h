#pragma once

#include "cutLeaderCommon.h"

#if defined(ClOption_Home)
#    define ClOption_Export _declspec(dllexport)
#else
#    define ClOption_Export _declspec(dllimport)
#endif
