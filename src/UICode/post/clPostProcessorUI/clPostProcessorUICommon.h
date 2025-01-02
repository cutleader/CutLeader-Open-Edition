#pragma once

#include "cutLeaderCommon.h"

#if defined(ClPostProcessorUI_Home)
#    define ClPostProcessorUI_Export _declspec(dllexport)
#else
#    define ClPostProcessorUI_Export _declspec(dllimport)
#endif

typedef enum tagMacRouterActionType
{
	MACROUTER_NC_SIMULATE	= 200,
} MACROUTER_ACTION_TYPE;

