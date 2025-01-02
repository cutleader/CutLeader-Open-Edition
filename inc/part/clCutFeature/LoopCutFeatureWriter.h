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
	// 保存“回路制造特征”。
	// 注：
	//   1) 这个函数不负责删除数据。
	static void SaveLoopFeature(DBConnectPtr pDBConnect, LONGLONG iPartCamDataId, LoopCutFeatureListPtr pLoopFeatureList);

	// 保存“轮廓刀具”。
	// 注：
	//   1) 这个函数不负责删除数据。
	static void UpdateLoopTool(DBConnectPtr pDBConnect, LoopToolDataPtr pLoopTool);

	// 保存“回路起切特征”。
	// 注：
	//   1) 这个函数不负责删除数据。
	static void UpdateLoopStaEndCut(DBConnectPtr pDBConnect, LoopStartCutDataPtr pLoopStartCutData);

	// 保存“回路微连接特征”。
	// 注：
	//   1) 这个函数不负责删除数据。
	static void UpdateLoopMicroJoint(DBConnectPtr pDBConnect, LoopMicroJointPtr pLoopMicroJoint);

	// 保存“回路角特征”。
	// 注：
	//   1) 这个函数不负责删除数据。
	static void UpdateLoopCorner(DBConnectPtr pDBConnect, LoopCornerDataPtr pLoopCorner);

private:
	static void UpdateLeadInfo(DBConnectPtr pDBConnect, ILeadPtr pLeadInfo, LONGLONG iLoopStaEndCutId);
};

END_CUTLEADER_NAMESPACE()
