#pragma once

#include "clExpertLibCommon.h"
#include "clCutFeatureCommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(ILead)
DECLARE_CUTLEADER_CLASS(LeadList)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(LeadConfigItem)

BEGIN_CUTLEADER_NAMESPACE()

// “引入引出线”管理器。
class ClCutFeature_Export LeadManager
{
	friend class LoopStartCutProcessor;

public:
	/************************************************************************/
	// 关于创建引入引出线。

	// 从老的引入引出线创建新的引入引出线。
	// notes: 
	//   1) we do not create the new copy of condition nodes.
	static ILeadPtr BuildLead(const ILead* pOldLead, int iPropName, double dVal);

	// 创建回路非角上的引入引出线。
	// 注:
	//   1) 调用该函数前需要设置好轮廓的切割偏移方式，该函数根据阴切阳切来分配引线，cut-on时分配NoneLead。
	static ILeadPtr BuildGeomLead_byLeadConfig(const IPatternLoop* pPatternLoop, PatternLoopCutOffsetType emLoopCutOffset, LeadInOutType emLeadInOut, const LeadConfigItem* pLeadConfigItem);

	// 创建引入引出线。
	// 注：
	//   1) 不设置切割条件。
	//   2) 这个函数已经指定了是引入还是引出线，以及线型，其他参数从参数配置中读取。
	static ILeadPtr BuildLead(LeadInOutType iLeadType, LeadType emLeadInOut, BOOL bBoundary, const LeadConfigItem* pLeadConfigItem);

	// 用专家库的配置来创建角上的引入引出线。
    // 注：
    //  1) 调用此函数时，iCutSide用来判断用延长线引线还是平分线引线，iCutSide不能是cutOn。
	static ILeadPtr BuildCornerLead_byLeadConfig(const IPatternLoop* pPatternLoop, BOOL bFollowDir, PatternLoopCutOffsetType iLoopCutOffset,
        LeadInOutType emLeadInOut, const Point2D& cornerPt, const LeadConfigItem* pLeadConfigItem);

	// 指定引线长度来创建角上的引入引出线。
    // 注：
    //  1) 调用此函数时，iCutSide用来判断用延长线引线还是平分线引线，iCutSide不能是cutOn。
	static ILeadPtr BuildCornerLead_byLeadLength(const IPatternLoop* pPatternLoop, BOOL bFollowDir, PatternLoopCutOffsetType iLoopCutOffset,
        LeadInOutType emLeadInOut, const Point2D& cornerPt, double dLeadLength);
	/************************************************************************/


	// 得到引入/引出线中指定的属性值。
	// 注：
	//   1) 调用者需要确定“pLead”中有属性“iPropName”。
	static double GetLeadPropVal(const ILead* pLead, int iPropName);

	// 这些引入/引出线中指定的属性值是否都等于“dVal”。
	// 注：
	//   1) 这些引入/引出线的类型一样。
	//   2) 调用者需要确定这些引入/引出线中有属性“iPropName”。
	static BOOL HaveSameVal(const LeadList* pLeadList, int iPropName, double dVal);

	// get the hull of the lead in or lead out.
	// notes:
	//   1) if no lead in or lead out, return null.
	//   2) this function will use the cache of start/end cut data.
	static Polygon2DPtr GetLeadHull(const LoopStartCutData* pLoopStartCutData, LeadInOutType emLeadInOut);

	// 修改引入引出线的长度。
	// 注：
	//  1) 没有引入引出线或者长度为零时，不做调整。
	//  2) 这个函数没有更新缓存。
	static void ChangeLeadLength(LoopStartCutDataPtr pLoopStartCutData, LeadInOutType leadtype, double dLength);

private:
	// about get the line/arcs of lead in/out.
	// notes: 
	//   1) for open path or Lead_None type lead, return empty list.
	//   2) if the lead in/out are at corner, we use a special process.
	//   3) for array pattern loop, only care the first pattern inst.
	//   4) call this function in LoopStartCutProcessor::UpdateCache and do not call this at other place.
	static LineArc2DListPtr CalcLeadBaseGeom(const LoopStartCutData* pLoopStartCutData, LeadInOutType leadtype);


	/************************************************************************/
	// get line/arc for all type lead.
	// notes:
	//   1) these functions will use the cache of start/end cut data.

	// get line/arc for "line" type lead.
	static LineArc2DListPtr CalcLineLeadGeom(const LoopStartCutData* pLoopStartCutData, LeadInOutType leadtype);

	// get line/arc for "two line" type lead.
	// notes:
	//   1) currently this lead type only support lead in, so this function cannot process lead out.
	static LineArc2DListPtr CalcTwoLineLeadGeom(const LoopStartCutData* pLoopStartCutData, LeadInOutType leadtype);

	// get line/arc for "arc" type lead.
	static LineArc2DListPtr CalcArcLeadGeom(const LoopStartCutData* pLoopStartCutData, LeadInOutType leadtype);

	// get line/arc for "line arc" type lead.
	static LineArc2DListPtr CalcLineArcLeadGeom(const LoopStartCutData* pLoopStartCutData, LeadInOutType leadtype);

	// get line/arc for "hook" type lead.
	static LineArc2DListPtr CalcHookLeadGeom(const LoopStartCutData* pLoopStartCutData, LeadInOutType leadtype);	
	/************************************************************************/


	// 修改引入引出线的长度。
	// 注：
	//  1) 这几个函数没有更新缓存。
	static void ChangeTwoLineLeadLen(LoopStartCutDataPtr pLoopStartCutData, LeadInOutType leadtype, double dLength);
	static void ChangeArcLeadLen(LoopStartCutDataPtr pLoopStartCutData, LeadInOutType leadtype, double dLength);
	static void ChangeLineArcLeadLen(LoopStartCutDataPtr pLoopStartCutData, LeadInOutType leadtype, double dLength);
	static void ChangeHookLeadLen(LoopStartCutDataPtr pLoopStartCutData, LeadInOutType leadtype, double dLength);
};

END_CUTLEADER_NAMESPACE()
