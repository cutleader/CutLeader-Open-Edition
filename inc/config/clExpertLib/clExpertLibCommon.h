#pragma once

#include "cutLeaderCommon.h"

#if defined(ClExpertLib_Home)
#    define ClExpertLib_Export _declspec(dllexport)
#else
#    define ClExpertLib_Export _declspec(dllimport)
#endif

// lead in or lead out.
enum LeadInOutType
{
	LeadInOut_In		= 0,
	LeadInOut_Out		= 1
};

// the lead type
enum LeadType
{
	Lead_None		= 0,
	Lead_Line		= 1,
	Lead_TwoLines	= 2,
	Lead_Arc		= 3,
	Lead_LineArc	= 4,
	Lead_Hook		= 5,
};

// the corner feature type.
enum CornerType
{
	Corner_None			= 0,
	Corner_Cross		= 1,
	Corner_Radius		= 2,
	Corner_Tab			= 3,
	Corner_Loop			= 4,
    Corner_CoolingPoint	= 5,
};

// the sequence policies.
enum SequencePolicyType
{	
	SequencePolicy_ShortestPath		= 0,
	SequencePolicy_DirectionX		= 1,
	SequencePolicy_DirectionY		= 2,
};

// �ο��㴦�ڵ�ͼͼԪ��Ӿ��ε��ĸ��ǡ�
enum CanvasRefPtPosition
{
	CanvasRefPt_LeftBottom = 1,	// ͼԪ���½ǡ�
	CanvasRefPt_RightBottom = 2,	// ͼԪ���½ǡ�
	CanvasRefPt_LeftTop = 3,	// ͼԪ���Ͻǡ�
	CanvasRefPt_RightTop = 4,	// ͼԪ���Ͻǡ�
	CanvasRefPt_CenterPt = 5,	// ͼԪ���ĵ㡣
};
