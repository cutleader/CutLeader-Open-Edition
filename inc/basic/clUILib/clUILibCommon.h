#pragma once

#include "cutLeaderCommon.h"

#if defined(ClUILib_Home)
#    define ClUILib_Export _declspec(dllexport)
#else
#    define ClUILib_Export _declspec(dllimport)
#endif

// the in-place editor type.
enum CELL_EDITOR_TYPE
{
	READEDITBOX		= 0, // the read only.
	STREDITBOX		= 1,
	INTEDITBOX		= 2,
	FLOATEDITBOX	= 3,
	COMBOBOX		= 4,
	COMBOEDIT		= 5,
};

// the types of units, these type are only used for dimension.
enum DIM_UNIT_TYPE
{
    DIM_UNIT_INCH  = 0,
    DIM_UNIT_MM    = 1,
};

// the NONE action type, under this action, user can do nothing.
enum NONE_ACTION_TYPE
{
    ACTION_NONE				= -1,
};

enum VIEW_ACTION_TYPE
{
    ACTION_ZOOM_IN			= -2,
    ACTION_ZOOM_OUT			= -3,
};
