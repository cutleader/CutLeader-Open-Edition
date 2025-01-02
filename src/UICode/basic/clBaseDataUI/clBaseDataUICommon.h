#pragma once

#include "cutLeaderCommon.h"

#if defined(ClBaseDataUI_Home)
#    define ClBaseDataUI_Export _declspec(dllexport)
#else
#    define ClBaseDataUI_Export _declspec(dllimport)
#endif
