#pragma once

#include "cutLeaderCommon.h"

#if defined(ClNestUI_Home)
#    define ClNestUI_Export _declspec(dllexport)
#else
#    define ClNestUI_Export _declspec(dllimport)
#endif
