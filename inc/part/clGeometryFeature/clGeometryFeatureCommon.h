#pragma once

#include "cutLeaderCommon.h"

#if defined(ClGeometryFeature_Home)
#    define ClGeometryFeature_Export _declspec(dllexport)
#else
#    define ClGeometryFeature_Export _declspec(dllimport)
#endif

// 对齐轮廓的方式。
enum AlignLoopStyle
{
	AlignLoop_Left = 0,
	AlignLoop_Right	= 1,
	AlignLoop_Top = 2,
	AlignLoop_Bottom = 3,
	AlignLoop_HorizontalCenter = 4,
	AlignLoop_VerticalCenter = 5,
	AlignLoop_Center = 6,
};
