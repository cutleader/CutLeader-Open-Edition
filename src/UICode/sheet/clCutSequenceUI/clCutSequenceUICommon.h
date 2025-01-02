#pragma once

#include "cutLeaderCommon.h"

#if defined(ClCutSequenceUI_Home)
#    define ClCutSequenceUI_Export _declspec(dllexport)
#else
#    define ClCutSequenceUI_Export _declspec(dllimport)
#endif

// 这些动作用来处理工序。
enum SequenceActionType
{
	SEQUENCE_SELECT				= 100,
	SEQUENCE_REMOVE				= 101,
	SEQUENCE_SORT_A				= 102, // 从头开始调整工序的顺序。
	SEQUENCE_SORT_B				= 103, // 调整选中工序的顺序。
	SEQUENCE_REPLACE_HOLE_SEQ	= 104,

	// 关于“回路工序”。
	SEQUENCE_ADD_LOOPSEQ		= 110,
	SEQUENCE_INSERT_LOOPSEQ		= 111,

	// 关于“桥接工序”。
	SEQUENCE_ADD_BDGESEQ		= 120,
	SEQUENCE_BREAK_BDGESEQ		= 121,
	SEQUENCE_BDGE_START_CHANGE	= 122,

	// 关于“连割工序”。
    SEQUENCE_ADD_RESET_INSTRUCT	= 123, // 添加复位指令
    SEQUENCE_DEL_RESET_INSTRUCT	= 124, // 删除复位指令
	SEQUENCE_ADD_CHAINSEQ		= 125,
	SEQUENCE_INSERT_CHAINNODE	= 126,
	SEQUENCE_DEL_CHAINNODE		= 127,

	// 关于“田字工序”。
	SEQUENCE_ADD_GRIDSEQ		= 128,

	// 关于“飞行切割工序”。
	SEQUENCE_Add_FLYCUT			= 130,

	// 关于编辑“运动节点”。
	SEQUENCE_ADD_MOV_PT			= 140,
	SEQUENCE_DEL_MOV_PT			= 141,

	// 余料切割线
	SEQUENCE_AUTO_REM_CUTLINE	= 145,
	SEQUENCE_ADD_REM_CUTLINE	= 146,
	SEQUENCE_DEL_REM_CUTLINE	= 147,

	SEQUENCE_VIEW				= 150,
};
