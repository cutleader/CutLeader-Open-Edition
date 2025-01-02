#pragma once

#include "cutLeaderCommon.h"

#if defined(ClTask_Home)
#    define ClTask_Export _declspec(dllexport)
#else
#    define ClTask_Export _declspec(dllimport)
#endif