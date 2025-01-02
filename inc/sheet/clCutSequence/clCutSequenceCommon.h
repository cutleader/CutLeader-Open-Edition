#pragma once

#include "cutLeaderCommon.h"

#if defined(ClCutSequence_Home)
#    define ClCutSequence_Export _declspec(dllexport)
#else
#    define ClCutSequence_Export _declspec(dllimport)
#endif


typedef enum tagLineSide
{
	SIDE_LEFT	= 0,
	SIDE_CENTER	= 1,
	SIDE_RIGHT	= 2,
} LINE_SIDE;

typedef enum tagArcSide
{
	SIDE_OUTSIDE	= 0,
	SIDE_ON			= 1,
	SIDE_INSIDE		= 2,
} ARC_SIDE;