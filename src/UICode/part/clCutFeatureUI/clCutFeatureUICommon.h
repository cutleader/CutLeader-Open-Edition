#pragma once

#include "cutLeaderCommon.h"

#if defined(ClCutFeatureUI_Home)
#    define ClCutFeatureUI_Export _declspec(dllexport)
#else
#    define ClCutFeatureUI_Export _declspec(dllimport)
#endif

enum CUTFEATURE_ACTION_TYPE
{
	LOOPTOOL_EDIT		= 97,
	LEAD_EDIT			= 98,
	START_PT_CHANGE		= 99,
	CUTSIDE_CHANGE		= 100,
	DIRECTION_CHANGE	= 101,

	MicroJoint_ADD		= 102,
	MicroJoint_EDIT		= 103,
	MicroJoint_DELETE	= 104,

	CORNER_ADD			= 106,
	CORNER_EDIT			= 107,
	CORNER_DELETE		= 108,
};
