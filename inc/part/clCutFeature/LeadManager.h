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

// �����������ߡ���������
class ClCutFeature_Export LeadManager
{
	friend class LoopStartCutProcessor;

public:
	/************************************************************************/
	// ���ڴ������������ߡ�

	// ���ϵ����������ߴ����µ����������ߡ�
	// notes: 
	//   1) we do not create the new copy of condition nodes.
	static ILeadPtr BuildLead(const ILead* pOldLead, int iPropName, double dVal);

	// ������·�ǽ��ϵ����������ߡ�
	// ע:
	//   1) ���øú���ǰ��Ҫ���ú��������и�ƫ�Ʒ�ʽ���ú������������������������ߣ�cut-onʱ����NoneLead��
	static ILeadPtr BuildGeomLead_byLeadConfig(const IPatternLoop* pPatternLoop, PatternLoopCutOffsetType emLoopCutOffset, LeadInOutType emLeadInOut, const LeadConfigItem* pLeadConfigItem);

	// �������������ߡ�
	// ע��
	//   1) �������и�������
	//   2) ��������Ѿ�ָ���������뻹�������ߣ��Լ����ͣ����������Ӳ��������ж�ȡ��
	static ILeadPtr BuildLead(LeadInOutType iLeadType, LeadType emLeadInOut, BOOL bBoundary, const LeadConfigItem* pLeadConfigItem);

	// ��ר�ҿ���������������ϵ����������ߡ�
    // ע��
    //  1) ���ô˺���ʱ��iCutSide�����ж����ӳ������߻���ƽ�������ߣ�iCutSide������cutOn��
	static ILeadPtr BuildCornerLead_byLeadConfig(const IPatternLoop* pPatternLoop, BOOL bFollowDir, PatternLoopCutOffsetType iLoopCutOffset,
        LeadInOutType emLeadInOut, const Point2D& cornerPt, const LeadConfigItem* pLeadConfigItem);

	// ָ�����߳������������ϵ����������ߡ�
    // ע��
    //  1) ���ô˺���ʱ��iCutSide�����ж����ӳ������߻���ƽ�������ߣ�iCutSide������cutOn��
	static ILeadPtr BuildCornerLead_byLeadLength(const IPatternLoop* pPatternLoop, BOOL bFollowDir, PatternLoopCutOffsetType iLoopCutOffset,
        LeadInOutType emLeadInOut, const Point2D& cornerPt, double dLeadLength);
	/************************************************************************/


	// �õ�����/��������ָ��������ֵ��
	// ע��
	//   1) ��������Ҫȷ����pLead���������ԡ�iPropName����
	static double GetLeadPropVal(const ILead* pLead, int iPropName);

	// ��Щ����/��������ָ��������ֵ�Ƿ񶼵��ڡ�dVal����
	// ע��
	//   1) ��Щ����/�����ߵ�����һ����
	//   2) ��������Ҫȷ����Щ����/�������������ԡ�iPropName����
	static BOOL HaveSameVal(const LeadList* pLeadList, int iPropName, double dVal);

	// get the hull of the lead in or lead out.
	// notes:
	//   1) if no lead in or lead out, return null.
	//   2) this function will use the cache of start/end cut data.
	static Polygon2DPtr GetLeadHull(const LoopStartCutData* pLoopStartCutData, LeadInOutType emLeadInOut);

	// �޸����������ߵĳ��ȡ�
	// ע��
	//  1) û�����������߻��߳���Ϊ��ʱ������������
	//  2) �������û�и��»��档
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


	// �޸����������ߵĳ��ȡ�
	// ע��
	//  1) �⼸������û�и��»��档
	static void ChangeTwoLineLeadLen(LoopStartCutDataPtr pLoopStartCutData, LeadInOutType leadtype, double dLength);
	static void ChangeArcLeadLen(LoopStartCutDataPtr pLoopStartCutData, LeadInOutType leadtype, double dLength);
	static void ChangeLineArcLeadLen(LoopStartCutDataPtr pLoopStartCutData, LeadInOutType leadtype, double dLength);
	static void ChangeHookLeadLen(LoopStartCutDataPtr pLoopStartCutData, LeadInOutType leadtype, double dLength);
};

END_CUTLEADER_NAMESPACE()
