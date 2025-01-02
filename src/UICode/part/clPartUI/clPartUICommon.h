#pragma once

#include "cutLeaderCommon.h"

#if defined(ClPartUI_Home)
#    define ClPartUI_Export _declspec(dllexport)
#else
#    define ClPartUI_Export _declspec(dllimport)
#endif