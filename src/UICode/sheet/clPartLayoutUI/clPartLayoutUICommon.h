#pragma once

#include "cutLeaderCommon.h"

#if defined(ClPartLayoutUI_Home)
#    define ClPartLayoutUI_Export _declspec(dllexport)
#else
#    define ClPartLayoutUI_Export _declspec(dllimport)
#endif

enum EDIT_SHEET_ACTION_TYPE
{
	// 零件布局。
	EDIT_ADDPARTPMT		= 100,
	EDIT_ADDPARTPMT_EX	= 101,
	EDIT_SELECTPRTPMT	= 110,
	EDIT_SELECTPRTINST	= 111,
	EDIT_DELETEPARTPMT	= 112,
	EDIT_ROTATEPARTPMT	= 113,
	EDIT_MOVEPRTPMT		= 114,
	EDIT_COPYPRTPMT		= 115,
	EDIT_GRIDPRTPMT		= 116,
	EDIT_EXPLODE_GRID	= 117,
	EDIT_ALLMOVEPRTPMT	= 118,

	// 制造工艺。
	LEAD_ADJUST			= 119,
	START_PT_ADJUST		= 120,
	CUTSIDE_ADJUST		= 121,
	DIRECTION_ADJUST	= 123,
	PMT_MJ_ADD			= 124,
	PMT_MJ_EDIT			= 125,
	PMT_MJ_DELETE		= 126,
	PMT_CORNER_ADD		= 127,
	PMT_CORNER_EDIT		= 128,
	PMT_CORNER_DELETE	= 129,
	LOOPTOOL_ADJUST		= 131,
    LOOPTOOL_ADJUST_2	= 132,

	// 测量距离。
	MEASURE_PT_DISTANCE	= 140,

    EDIT_CreatePartGroup	= 150,
    EDIT_ExplodePartGroup	= 151,
    EDIT_SelectPartOrLoopInstance	= 152, // 编辑器使用上一次的选择状态
    EDIT_SelectLoopInstance	= 153,
};