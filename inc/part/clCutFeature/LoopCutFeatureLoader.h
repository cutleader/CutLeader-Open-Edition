#pragma once

#include "clCutFeatureCommon.h"
#include <tuple>

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopToolData)
DECLARE_CUTLEADER_CLASS(LoopToolDataList)
DECLARE_CUTLEADER_CLASS(ConditionNodeList)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(LoopCutFeatureList)
DECLARE_CUTLEADER_CLASS(ILead)
DECLARE_CUTLEADER_CLASS(LeadCondDataList)
DECLARE_CUTLEADER_CLASS(LoopMicroJoint)
DECLARE_CUTLEADER_CLASS(LoopMicroJointList)
DECLARE_CUTLEADER_CLASS(MicroJoint)
DECLARE_CUTLEADER_CLASS(MicroJointList)
DECLARE_CUTLEADER_CLASS(CornerList)
DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)
DECLARE_CUTLEADER_CLASS(LoopStaEndCutCondList)
DECLARE_CUTLEADER_CLASS(LeadList)
DECLARE_CUTLEADER_CLASS(MicroJointDataList)
DECLARE_CUTLEADER_CLASS(MicroJointCondDataList)
DECLARE_CUTLEADER_CLASS(LoopCornerData)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutFeature_Export LoopCutFeatureLoader
{
public:
	// load loop features of "iPartCamDataID".
	// ע:
	//   1) ���ĳ������·������������Ч�ˣ�������Ϊ�գ���������Ҫ�ں�������������
	//   2) the cache data of loop feature will be updated.
	static LoopCutFeatureListPtr LoadLoopFeatures(DBConnectPtr pDBConnect, LONGLONG iCamDataID, PatternLoopListPtr pPatternLoopList);

    // ����ĳ�������ϵĵ�����Ϣ
    static std::tr1::tuple<CString, double> LoadToolInfo_of_loop(DBConnectPtr pDBConnect, LONGLONG iPartCamDataID, LONGLONG iPatternLoopID);

private:
	// ������������
	static LoopToolDataPtr LoadLoopTool(DBConnectPtr pDBConnect, IPatternLoopPtr pPatternLoop, LONGLONG iLoopToolID);
	static ConditionNodeListPtr LoadLoopConditionInfo(DBConnectPtr pDBConnect, LONGLONG iLoopToolID); // ���������ϵ�������Ϣ

	/************************************************************************/
	// ���ڡ���·������������

	// ���ء���·������������
	/* params:
	*  pLeadCondDataList: all lead CondNodes under current cam data set.
	*/
	// notes:
	//   1) update the cache data of loop start/end info.
	static LoopStartCutDataPtr LoadLoopStaEndCut(DBConnectPtr pDBConnect, IPatternLoopPtr pLoop, LONGLONG iLoopStaEndCutId, LeadCondDataListPtr pLeadCondDataList);

	// load all loop pierce data of the "iPartCamDataID".
	// ע:
	//   1) ���ĳ������·������������Ч�ˣ�ȥ������
	//   2) update the cache data of loop start/end info.
	static LoopStartCutDataListPtr LoadAllLoopStaEndCut(DBConnectPtr pDBConnect, LONGLONG iPartCamDataID, PatternLoopListPtr pPatternLoopList);

	// load LeadInOut object by id.
	static ILeadPtr LoadLeadInfo(DBConnectPtr pDBConnect, LONGLONG iId);

	// load 2 LeadInOut for the loop.
	/* params:
	*  pLeadCondDataList: all lead CondNodes under current cam data set.
	*/
	static LeadListPtr LoadLoopLeadInfo(DBConnectPtr pDBConnect, LONGLONG iLoopStaEndCutId, LeadCondDataListPtr pLeadCondDataList);

	// load all lead data of the "iPartCamDataID".
	// notes:
	//   1) call "LoadAllLeadCond" in this function.
	static LeadListPtr LoadAllLeadData(DBConnectPtr pDBConnect, LONGLONG iPartCamDataID);
	/************************************************************************/

	/************************************************************************/
	// ���ڡ���·΢������������

	static LoopMicroJointPtr LoadLoopMicroJoint(DBConnectPtr pDBConnect, IPatternLoopPtr pLoop, LONGLONG iLoopMicroJointId);

	// load all loop micro joint data of the part cam data set.
	// ע��
	//  1) Ҫ�ǻ�·û���ˣ�����΢��������ͼ��û���ˣ��ǾͶ���΢���ӡ�
	static LoopMicroJointListPtr LoadAllLoopMicroJoint(DBConnectPtr pDBConnect, LONGLONG iPartCamDataID, PatternLoopListPtr pPatternLoopList);

	static void LoadMicroJoints(DBConnectPtr pDBConnect, LONGLONG iLoopMicroJointid, MicroJointListPtr pMicroJointList);

	// load all micro joint object of the part cam data set.
	static MicroJointDataListPtr LoadAllMicroJoint(DBConnectPtr pDBConnect, LONGLONG iPartCamDataID);
	/************************************************************************/

	// ���ڡ���·����������
	// ע��
	//  1) ��Ч�ġ�����������Ҫɾ����
	static LoopCornerDataPtr LoadLoopCorner(DBConnectPtr pDBConnect, IPatternLoopPtr pPatternLoop, LONGLONG iLoopCornerID);
	static void LoadCorners(DBConnectPtr pDBConnect, LONGLONG iLoopCornerID, CornerListPtr pCornerList);
};

END_CUTLEADER_NAMESPACE()
