#pragma once

#include "cutLeaderCommon.h"

#if defined(ClCutFeature_Home)
#    define ClCutFeature_Export _declspec(dllexport)
#else
#    define ClCutFeature_Export _declspec(dllimport)
#endif


// the cut side of the pattern loop.
enum PatternLoopCutSideType
{
	PatternLoopCutSide_Left	    = 0,
	PatternLoopCutSide_Right	= 1,
	PatternLoopCutSide_On		= 2,
};

// 轮廓切割偏移类型，内外切和cut-on
enum PatternLoopCutOffsetType
{
	PatternLoopCutOffset_Inside		= 0,
	PatternLoopCutOffset_Outside	= 1,
	PatternLoopCutOffset_On			= 2,
};

// 修改轮廓切割方向的方式。
// 注：
// 1) 这个类型是面向交互的，数据层面的修改方式都是把切割方向反向一下。
enum CutDirectionChangeType
{
	CutDirectionChange_Switch	= 0,
	CutDirectionChange_CCW		= 1,
	CutDirectionChange_CW		= 2,
};
