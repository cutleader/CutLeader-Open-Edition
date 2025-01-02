#pragma once

#include "cutLeaderCommon.h"

#if defined(ClGeometryBase_Home)
#    define ClGeometryBase_Export _declspec(dllexport)
#else
#    define ClGeometryBase_Export _declspec(dllimport)
#endif

typedef enum tagDirection
{
	CW			= 0,
	CCW			= 1,
	UNKNOWN		= 2
} DIRECTION;

typedef enum tagPosition
{
	POSITION_LEFT		= 0,
	POSITION_RIGHT		= 1,
	POSITION_UP			= 2,
	POSITION_DOWN		= 3,
} POSITION_INFO;

typedef enum tagXYDirection
{
	XY_DIR_X			= 1,
	XY_DIR_Y			= 2,
} XY_DIRECTION;

/*
	3 4
	1 2
*/
typedef enum tagRectCorner
{	
	LEFT_TOP			= 3,
	RIGHT_TOP			= 4,
	LEFT_BOTTOM			= 1,
	RIGHT_BOTTOM		= 2,

} RECT_CORNER;