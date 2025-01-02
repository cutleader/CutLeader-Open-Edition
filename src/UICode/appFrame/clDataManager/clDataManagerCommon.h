#pragma once

#include "cutLeaderCommon.h"

#if defined(ClDataManager_Home)
#    define ClDataManager_Export _declspec(dllexport)
#else
#    define ClDataManager_Export _declspec(dllimport)
#endif