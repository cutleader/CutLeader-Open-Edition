#pragma once

#include "cutLeaderCommon.h"

#if defined(ClMachineLib_Home)
#    define ClMachineLib_Export _declspec(dllexport)
#else
#    define ClMachineLib_Export _declspec(dllimport)
#endif
