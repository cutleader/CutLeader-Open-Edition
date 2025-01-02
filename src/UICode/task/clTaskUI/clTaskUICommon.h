#pragma once

#include "cutLeaderCommon.h"

#if defined(ClTaskUI_Home)
#    define ClTaskUI_Export _declspec(dllexport)
#else
#    define ClTaskUI_Export _declspec(dllimport)
#endif