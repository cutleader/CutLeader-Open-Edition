#pragma once

#include "cutLeaderCommon.h"

#if defined(ClDrawWrapper_Home)
#    define ClDrawWrapper_Export _declspec(dllexport)
#else
#    define ClDrawWrapper_Export _declspec(dllimport)
#endif