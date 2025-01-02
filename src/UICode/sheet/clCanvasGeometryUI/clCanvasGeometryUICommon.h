#pragma once

#include "cutLeaderCommon.h"

#if defined(ClCanvasGeometryUI_Home)
#    define ClCanvasGeometryUI_Export _declspec(dllexport)
#else
#    define ClCanvasGeometryUI_Export _declspec(dllimport)
#endif

// 用于编辑底图上几何特征的action。
enum EditCanvasGeometryActionType
{
	EditLinePatternOnCanvas				= 160,
	EditArcPatternOnCanvas				= 161,
	EditMultiLinePatternOnCanvas		= 162,
	EditCirclePatternOnCanvas			= 163,
	EditRectPatternOnCanvas				= 164,
	DrawTextStructOnCanvas				= 165,
	SelectPatternOnCanvas				= 166,
	DeletePatternOnCanvas				= 167,
	CopyPatternOnCanvas					= 168,
	MovePatternOnCanvas					= 169,
	RotatePatternOnCanvas				= 170,
	GridPatternOnCanvas					= 171,
	AddPatternFromDxf					= 172,
	AddCanvasPattern2Task				= 173,
	EditRectLinePatternOnCanvas			= 174,
	AlignLoopsOnCanvas					= 175,
};
