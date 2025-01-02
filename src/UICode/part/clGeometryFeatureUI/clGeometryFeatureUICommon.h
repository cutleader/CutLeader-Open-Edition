#pragma once

#include "cutLeaderCommon.h"

#if defined(ClGeometryFeatureUI_Home)
#    define ClGeometryFeatureUI_Export _declspec(dllexport)
#else
#    define ClGeometryFeatureUI_Export _declspec(dllimport)
#endif

typedef enum tagDrawActionType
{
	DRAW_LINE			= 1,
	DRAW_MULTILINE		= 2,
	DRAW_RECTLINES		= 3,

	// hole
	DRAW_RECT_HOLE		= 4,
	DRAW_TR_HOLE		= 18,

	DRAW_ARC_STD		= 5,
	DRAW_ARC_3PT		= 6,
	DRAW_RC_HOLE		= 7,
	DRAW_RC_LINES		= 8,

	// round hole
	DRAW_ROUNDHOLE		= 9,
	DRAW_OBROUND		= 22,
	DRAW_ELLIPSE		= 10,

	// D shape
	DRAW_SINGLED		= 11,
	DRAW_DOUBLED		= 19,

	// array
	DRAW_PATTERNLINE	= 12,
	DRAW_PATTERNARC		= 31,
	DRAW_PATTERNCIRCLE	= 32,
	DRAW_PATTERNGRID	= 33,

	// radius slot
	DRAW_RADSLOT		= 30,

	DRAW_Text			= 34,
} DRAW_ACTION_TYPE;

typedef enum tagEditActionType
{
	EDIT_SELECTPATTERN  = 50,
	EDIT_SELECTPATLOOP  = 51,

	EDIT_DELETE			= 60,
	EDIT_EXPLODE		= 61,
	EDIT_COPY			= 62,
	EDIT_MOVE			= 63,
	EDIT_ROTATE			= 64,
	EDIT_OFFSET			= 65,
	AlignPatternLoop	= 66,
} EDIT_ACTION_TYPE;