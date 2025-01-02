#pragma once

#include "cutLeaderCommon.h"

#if defined(CLOPTIONUI_HOME)
#    define CLOPTIONUI_Export _declspec(dllexport)
#else
#    define CLOPTIONUI_Export _declspec(dllimport)
#endif
