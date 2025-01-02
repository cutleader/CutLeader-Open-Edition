#pragma once

#include "cutLeaderCommon.h"

#if defined(ClMachineLibUI_Home)
#    define ClMachineLibUI_Export _declspec(dllexport)
#else
#    define ClMachineLibUI_Export _declspec(dllimport)
#endif
