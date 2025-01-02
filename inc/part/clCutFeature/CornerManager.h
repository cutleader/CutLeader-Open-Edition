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

// 角特征管理器。
class ClCutFeature_Export CornerManager
{
public:
	/************************************************************************/
	// 关于创建“角特征”。

	// 在回路上指定的点创建“角特征”。
	static ICornerPtr BuildCorner(LONGLONG iLoopCornerID, const PolygonPatternLoop* pPolygonPatternLoop, const Point2D& cornerPt, CornerType type, double dValue);

	// 根据角类型和值创建“角特征”。
	static ICornerPtr BuildCorner(LONGLONG iLoopCornerID, CornerType iCornerType, double dVal, LONGLONG iFirstPatID, LONGLONG iSecondPatID);

	// 通过修改老的角特征值来新建“角特征”。
    // 注：
    //  1) pOldCorner需要有“角特征值”。
	static ICornerPtr BuildCorner_byChangeCornerValue(const ICorner* pOldCorner, double dVal);
	/************************************************************************/


	/************************************************************************/
	// 关于检查“角特征”。

	// 删除无效的“角特征”。
	// 注：
	//   1) 如果有无效的“角特征”就返回TRUE。
	static BOOL DelInvalidCorner(LoopCornerDataPtr pLoopCorner);

	// 检查“角特征值”能不能被接受。
	// 注:
	//   1) we should check whether value of radius/tab corner is reasonable.
	static BOOL CheckCornerValue(const IPatternLoop* pPatternLoop, const ICorner* pCorner);

	// 检查这些角特征的“角特征值”能不能被接受。
	// 注:
	//   1) we should check whether value of radius/tab corner is reasonable.
	static BOOL CheckCornerValue(const LoopCornerDataList* pLoopCornerList, const CornerList* pCornerList);
	/************************************************************************/


	/************************************************************************/
	// 关于“角特征值”。

    // 这种角特征有没有“角特征值”。
    static bool WhetherHaveCornerValue(CornerType iCornerType);

	// 得到角特征值。
	static double GetCornerValue(const ICorner* pCorner);

	// 设置角特征值。
	static void SetCornerValue(ICornerPtr pCorner, double dNewVal);

	// 这些角特征是不是有一样的值。
	static BOOL SameCornerValue(const CornerList* pCornerList);
	/************************************************************************/


	// 根据参数配置给回路分配“角特征”。
	// 注：
	// 1) 目前只能对封闭的多边形轮廓加角特征。
	static void AssignLoopCorner(LoopCornerDataPtr pLoopCorner, const LoopTopologyItemList* pLoopTopologyItems, const CornerConfigItem* pCornerConfigItem);
};

END_CUTLEADER_NAMESPACE()
