#pragma once

#include "cutLeaderCommon.h"

#if defined(ClDxfDwg_Home)
#    define ClDxfDwg_Export _declspec(dllexport)
#else
#    define ClDxfDwg_Export _declspec(dllimport)
#endif