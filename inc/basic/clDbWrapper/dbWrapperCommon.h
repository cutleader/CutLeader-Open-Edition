#pragma once

#include "cutLeaderCommon.h"

#if defined(ClDbWrapper_Home)
#    define ClDbWrapper_Export _declspec(dllexport)
#else
#    define ClDbWrapper_Export _declspec(dllimport)
#endif