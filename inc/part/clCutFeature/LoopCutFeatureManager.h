#pragma once

#include "clCutFeatureCommon.h"
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopCutFeature)
DECLARE_CUTLEADER_CLASS(LoopCutFeatureList)
DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// ����·�ӹ����ա���������
class ClCutFeature_Export LoopCutFeatureManager
{
public:
	// ������мӹ����յ������ԡ�
	/* params:
	*  bCheckValid: whether step 1 will be performed.
    *  transformMatInSheet: �Ӱ��ģʽ�µ����������ʱ�����͹�������ڰ���еı任����
	*/
	// ע: 
	//   1) �������:
	//      a) �ȼ����������Ƿ���Ч��Ҫ����Ч��ɾ��������StaEndCut�������ڵ�ͼ�ζ�ʧ�ˣ�ɾ��StaEndCut����
	//      b) ��������·������Ҫ��Ҫ����ӹ����ա�
	static void CheckIntegrity(const PatternLoopList* pPatternLoops, const LoopTopologyItemList* pLoopTopologyItems, LoopCutFeatureListPtr pLoopFeatures,
		const ParamConfig* pParamConfig, BOOL bCheckValid, BOOL bMicroJoint, BOOL bCorner, const Matrix2D& transformMatInSheet);

	// ����ר�ҿ��������·����ӹ����ա�
	/* params:
	*  bAssignMicroJointCorner: whether assign micro joint/Corner features.
	*/
	static void AssignFeatures(const PatternLoopList* pPatternLoops, const LoopTopologyItemList* pLoopTopologyItems, LoopCutFeatureListPtr pLoopFeatures,
		const ParamConfig* pParamConfig, BOOL bAssignMicroJointCorner, const Matrix2D& transformMatInSheet);

	// ���·��������·�ġ���·������������
    // ע��
    //  1) transformMatInSheet: �Ӱ��ģʽ�µ����������ʱ�����͹�������ڰ���еı任����
	static void ResetLoopFeature(PatternLoopListPtr pPatternLoops, const LoopTopologyItemList* pLoopTopologyItems, LoopCutFeatureListPtr pLoopFeatures,
		const ParamConfig* pParamConfig, const Matrix2D& transformMatInSheet);

	// ���ƫ�ƺ�������ϵļӹ����ա�
	static void Check4Offset(LoopCutFeaturePtr pLoopFeature);

	// ɾ����Ч�ļӹ����ա�
	static void DelInvalidFeature(LoopCutFeaturePtr pLoopFeature, const ParamConfig* pParamConfig);

	// ����ר�ҿ��ͼ�λ�·���䡰�ӹ����ա���
	static LoopCutFeaturePtr AssignLoopFeature(IPatternLoopPtr pPatternLoop, const LoopTopologyItemList* pLoopTopologyItems, const ParamConfig* pParamConfig,
		BOOL bMicroJoint, BOOL bCorner, const Matrix2D& transformMatInSheet);

	// ����·�ļӹ������Ƿ�������
	// ע��
	//   1) Ҫ�ǻ�·û�С����������������͡���ʼ�����и��Ϣ����ô����·��������������������Ҫ���ϡ�
	//   2) ��·����û�С�΢��������������������������ͼ�����������������ǡ�bMicroJoint��bCorner��bPatCond����ȷָ������Ҫ��Щ���ա�
	static void CheckIntegrity(LoopCutFeaturePtr pLoopFeature, const LoopTopologyItemList* pLoopTopologyItems, const ParamConfig* pParamConfig,
		BOOL bMicroJoint, BOOL bCorner, const Matrix2D& transformMatInSheet);
};

END_CUTLEADER_NAMESPACE()
