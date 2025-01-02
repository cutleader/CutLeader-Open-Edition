#pragma once

#include "clExpertLibCommon.h"
#include "clCutFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(Point2D)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PolygonPatternLoop)
DECLARE_CUTLEADER_CLASS(CornerConfigItem)
DECLARE_CUTLEADER_CLASS(CornerList)
DECLARE_CUTLEADER_CLASS(ICorner)
DECLARE_CUTLEADER_CLASS(LoopCornerData)
DECLARE_CUTLEADER_CLASS(LoopCornerDataList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// ��������������
class ClCutFeature_Export CornerManager
{
public:
	/************************************************************************/
	// ���ڴ���������������

	// �ڻ�·��ָ���ĵ㴴��������������
	static ICornerPtr BuildCorner(LONGLONG iLoopCornerID, const PolygonPatternLoop* pPolygonPatternLoop, const Point2D& cornerPt, CornerType type, double dValue);

	// ���ݽ����ͺ�ֵ����������������
	static ICornerPtr BuildCorner(LONGLONG iLoopCornerID, CornerType iCornerType, double dVal, LONGLONG iFirstPatID, LONGLONG iSecondPatID);

	// ͨ���޸��ϵĽ�����ֵ���½�������������
    // ע��
    //  1) pOldCorner��Ҫ�С�������ֵ����
	static ICornerPtr BuildCorner_byChangeCornerValue(const ICorner* pOldCorner, double dVal);
	/************************************************************************/


	/************************************************************************/
	// ���ڼ�顰����������

	// ɾ����Ч�ġ�����������
	// ע��
	//   1) �������Ч�ġ����������ͷ���TRUE��
	static BOOL DelInvalidCorner(LoopCornerDataPtr pLoopCorner);

	// ��顰������ֵ���ܲ��ܱ����ܡ�
	// ע:
	//   1) we should check whether value of radius/tab corner is reasonable.
	static BOOL CheckCornerValue(const IPatternLoop* pPatternLoop, const ICorner* pCorner);

	// �����Щ�������ġ�������ֵ���ܲ��ܱ����ܡ�
	// ע:
	//   1) we should check whether value of radius/tab corner is reasonable.
	static BOOL CheckCornerValue(const LoopCornerDataList* pLoopCornerList, const CornerList* pCornerList);
	/************************************************************************/


	/************************************************************************/
	// ���ڡ�������ֵ����

    // ���ֽ�������û�С�������ֵ����
    static bool WhetherHaveCornerValue(CornerType iCornerType);

	// �õ�������ֵ��
	static double GetCornerValue(const ICorner* pCorner);

	// ���ý�����ֵ��
	static void SetCornerValue(ICornerPtr pCorner, double dNewVal);

	// ��Щ�������ǲ�����һ����ֵ��
	static BOOL SameCornerValue(const CornerList* pCornerList);
	/************************************************************************/


	// ���ݲ������ø���·���䡰����������
	// ע��
	// 1) Ŀǰֻ�ܶԷ�յĶ���������ӽ�������
	static void AssignLoopCorner(LoopCornerDataPtr pLoopCorner, const LoopTopologyItemList* pLoopTopologyItems, const CornerConfigItem* pCornerConfigItem);
};

END_CUTLEADER_NAMESPACE()
