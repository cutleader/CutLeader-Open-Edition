#pragma once

#include "cutLeaderCommon.h"

#if defined(ClNestJob_Home)
#    define ClNestJob_Export _declspec(dllexport)
#else
#    define ClNestJob_Export _declspec(dllimport)
#endif

enum PartRotateType
{
	// part can be rotated freely.
	PartRotate_Free = 0,

	// part can be rotated by 90 degree increment, that is, part can be placed in sheet with 0/90/180/270 degree.
	PartRotate_90Increment = 1,

	// part can be rotated by 0 degree.
	PartRotate_0Fixed = 2,

	// part can be rotated by 90 degree.
	PartRotate_90Fixed = 3,

	// part can be rotated by 180 degree.
	PartRotate_180Fixed = 4,

	// part can be rotated by 270 degree.
	PartRotate_270Fixed = 5,

	// 0度、180度旋转。
	PartRotate_0And180Fixed = 6,

	// 90度、270度旋转。
	PartRotate_90And270Fixed = 7,
};