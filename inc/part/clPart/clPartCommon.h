#pragma once

#include "cutLeaderCommon.h"

#if defined(ClPart_Home)
#    define ClPart_Export _declspec(dllexport)
#else
#    define ClPart_Export _declspec(dllimport)
#endif