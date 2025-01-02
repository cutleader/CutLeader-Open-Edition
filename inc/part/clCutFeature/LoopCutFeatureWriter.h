#pragma once

#include "clCutFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(LoopCutFeatureList)
DECLARE_CUTLEADER_CLASS(LoopToolData)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(ILead)
DECLARE_CUTLEADER_CLASS(LoopCornerData)
DECLARE_CUTLEADER_CLASS(MicroJoint)
DECLARE_CUTLEADER_CLASS(LoopMicroJoint)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutFeature_Export LoopCutFeatureWriter
{
public:
	// ���桰��·������������
	// ע��
	//   1) �������������ɾ�����ݡ�
	static void SaveLoopFeature(DBConnectPtr pDBConnect, LONGLONG iPartCamDataId, LoopCutFeatureListPtr pLoopFeatureList);

	// ���桰�������ߡ���
	// ע��
	//   1) �������������ɾ�����ݡ�
	static void UpdateLoopTool(DBConnectPtr pDBConnect, LoopToolDataPtr pLoopTool);

	// ���桰��·������������
	// ע��
	//   1) �������������ɾ�����ݡ�
	static void UpdateLoopStaEndCut(DBConnectPtr pDBConnect, LoopStartCutDataPtr pLoopStartCutData);

	// ���桰��·΢������������
	// ע��
	//   1) �������������ɾ�����ݡ�
	static void UpdateLoopMicroJoint(DBConnectPtr pDBConnect, LoopMicroJointPtr pLoopMicroJoint);

	// ���桰��·����������
	// ע��
	//   1) �������������ɾ�����ݡ�
	static void UpdateLoopCorner(DBConnectPtr pDBConnect, LoopCornerDataPtr pLoopCorner);

private:
	static void UpdateLeadInfo(DBConnectPtr pDBConnect, ILeadPtr pLeadInfo, LONGLONG iLoopStaEndCutId);
};

END_CUTLEADER_NAMESPACE()
