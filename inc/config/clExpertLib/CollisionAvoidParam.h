#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(CollisionAvoidParam)

BEGIN_CUTLEADER_NAMESPACE()

// �������е�ķ�ʽ
enum AdjustStartCutPtType
{
	AdjustInnerLoopsOnly	= 0, // ֻ�����ڿ׵����е�
	AdjustAllLoops			= 1, // �����ڿ׺������������е�
};

// ��ײ������򡣰����˵������е�����ɱ���·�������ӹ���
class ClExpertLib_Export CollisionAvoidParam : public ClData
{
public:
	CollisionAvoidParam(void);
	~CollisionAvoidParam(void);

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // get/set functions.
	void SetIsAdjustStartCutPt(BOOL bAdjustStartCutPt) { m_bAdjustStartCutPt = bAdjustStartCutPt; }
	BOOL GetIsAdjustStartCutPt() const { return m_bAdjustStartCutPt; }

	void SetAdjustStartCutPtType(AdjustStartCutPtType iAdjustStartCutPtType) { m_iAdjustStartCutPtType = iAdjustStartCutPtType; }
	AdjustStartCutPtType GetAdjustStartCutPtType() const { return m_iAdjustStartCutPtType; }

	void SetIsGenerateAvoidPath(BOOL bGenerateAvoidPath) { m_bGenerateAvoidPath = bGenerateAvoidPath; }
	BOOL GetIsGenerateAvoidPath() const { return m_bGenerateAvoidPath; }

	void SetCutOutAvoid(double dCutOutAvoid) { m_dCutOutAvoid = dCutOutAvoid; }
	double GetCutOutAvoid() const { return m_dCutOutAvoid; }

	void SetAvoidDis(double dAvoidDis) { m_dAvoidDis = dAvoidDis; }
	double GetAvoidDis() const { return m_dAvoidDis; }

	void SetAvoidRatio(double dAvoidRatio) { m_dAvoidRatio = dAvoidRatio; }
	double GetAvoidRatio() const { return m_dAvoidRatio; }

private:
	/************************************************************************/
	// ���������е㡱�ӹ���

	// �Ƿ�����
	BOOL m_bAdjustStartCutPt;

	// �������е�ķ�ʽ
	AdjustStartCutPtType m_iAdjustStartCutPtType;
	/************************************************************************/

	/************************************************************************/
	// �����ɱ���·�����ӹ���

	// �Ƿ�����
	BOOL m_bGenerateAvoidPath;

	// this variable indicate the cutouts we should avoid.
	double m_dCutOutAvoid;

	// the avoid distance, this defines the distance of the cut header to the actual cutout.
	// notes:
	//   1) use this value to calculate AvoidRect object. please refer to AvoidRectInfo class.
	double m_dAvoidDis;

	// the cut avoid ratio.
	// notes:
	//   1) use this value to calculate the max rect range which contains the trace line path. e,g, this statement will calculate the half
	//      width of the range rect: "startPt.DistanceTo(endPt)*0.5*pCollisionAvoidParam->m_dAvoidRatio"
	double m_dAvoidRatio;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
