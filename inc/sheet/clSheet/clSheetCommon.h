#pragma once

#include "cutLeaderCommon.h"

#if defined(ClSheet_Home)
#    define ClSheet_Export _declspec(dllexport)
#else
#    define ClSheet_Export _declspec(dllimport)
#endif