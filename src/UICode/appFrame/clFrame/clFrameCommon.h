#pragma once

#include "cutLeaderCommon.h"

#if defined(CLFRAME_HOME)
#    define CLFRAME_Export _declspec(dllexport)
#else
#    define CLFRAME_Export _declspec(dllimport)
#endif