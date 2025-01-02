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

// “回路加工工艺”管理器。
class ClCutFeature_Export LoopCutFeatureManager
{
public:
	// 检查所有加工工艺的完整性。
	/* params:
	*  bCheckValid: whether step 1 will be performed.
    *  transformMatInSheet: 从板材模式下调用这个函数时，会送过来零件在板材中的变换矩阵。
	*/
	// 注: 
	//   1) 检查流程:
	//      a) 先检查各个工艺是否还有效，要是无效就删掉，比如StaEndCut对象所在的图形丢失了，删除StaEndCut对象。
	//      b) 检查各个回路，看看要不要分配加工工艺。
	static void CheckIntegrity(const PatternLoopList* pPatternLoops, const LoopTopologyItemList* pLoopTopologyItems, LoopCutFeatureListPtr pLoopFeatures,
		const ParamConfig* pParamConfig, BOOL bCheckValid, BOOL bMicroJoint, BOOL bCorner, const Matrix2D& transformMatInSheet);

	// 根据专家库给各个回路分配加工工艺。
	/* params:
	*  bAssignMicroJointCorner: whether assign micro joint/Corner features.
	*/
	static void AssignFeatures(const PatternLoopList* pPatternLoops, const LoopTopologyItemList* pLoopTopologyItems, LoopCutFeatureListPtr pLoopFeatures,
		const ParamConfig* pParamConfig, BOOL bAssignMicroJointCorner, const Matrix2D& transformMatInSheet);

	// 重新分配各个回路的“回路制造特征”。
    // 注：
    //  1) transformMatInSheet: 从板材模式下调用这个函数时，会送过来零件在板材中的变换矩阵。
	static void ResetLoopFeature(PatternLoopListPtr pPatternLoops, const LoopTopologyItemList* pLoopTopologyItems, LoopCutFeatureListPtr pLoopFeatures,
		const ParamConfig* pParamConfig, const Matrix2D& transformMatInSheet);

	// 检查偏移后的轮廓上的加工工艺。
	static void Check4Offset(LoopCutFeaturePtr pLoopFeature);

	// 删除无效的加工工艺。
	static void DelInvalidFeature(LoopCutFeaturePtr pLoopFeature, const ParamConfig* pParamConfig);

	// 根据专家库给图形回路分配“加工工艺”。
	static LoopCutFeaturePtr AssignLoopFeature(IPatternLoopPtr pPatternLoop, const LoopTopologyItemList* pLoopTopologyItems, const ParamConfig* pParamConfig,
		BOOL bMicroJoint, BOOL bCorner, const Matrix2D& transformMatInSheet);

	// 检查回路的加工工艺是否完整。
	// 注：
	//   1) 要是回路没有“轮廓刀具特征”和“开始结束切割”信息，那么“回路制造特征”不完整，需要补上。
	//   2) 回路可以没有“微连接特征”，“角特征”，“图形条件特征”，除非“bMicroJoint，bCorner，bPatCond”明确指定了需要这些工艺。
	static void CheckIntegrity(LoopCutFeaturePtr pLoopFeature, const LoopTopologyItemList* pLoopTopologyItems, const ParamConfig* pParamConfig,
		BOOL bMicroJoint, BOOL bCorner, const Matrix2D& transformMatInSheet);
};

END_CUTLEADER_NAMESPACE()
