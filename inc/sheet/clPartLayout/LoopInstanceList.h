#pragma once

#include "clPartLayoutCommon.h"
#include "ClData.h"
#include "Rect2D.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(MicroJoint)
DECLARE_CUTLEADER_CLASS(ICorner)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(LoopToolData)
DECLARE_CUTLEADER_CLASS(LoopToolDataList)


BEGIN_CUTLEADER_NAMESPACE()


// “回路实例”集合。
class ClPartLayout_Export LoopInstanceList : public std::vector<LoopInstancePtr>
{
public:
	LoopInstanceList(void);
	~LoopInstanceList(void);

public:
	/************************************************************************/
	// 关于得到回路实例。

	// 根据几何轮廓ID得到轮廓实例。有可能不止一个轮廓实例匹配。
	LoopInstancePtr GetLoopInstanceByPatternLoop(LONGLONG iPatternLoopID) const;

	// get the loop instance by the loop inst ID.
	// notes:
	//   1) 轮廓实例ID不是一个持久的数据，调用该函数时需注意。
	LoopInstancePtr GetLoopInstanceByID(LONGLONG iLoopInstID);

    // 根据零件实例和几何轮廓得到轮廓实例。
    LoopInstancePtr GetLoopInstance_ByPartInstanceAndPatternLoop(LONGLONG iPartInstanceID, LONGLONG iPatternLoopID) const;

	// whether this list includes "pLoopInstance".
	// notes: 
	//   1) we do not use the ID if loop inst because loop instances are create dynamically.
	BOOL IncludeLoopInstance(const LoopInstance* pLoopInstance, int& iIndex) const;
	BOOL IncludeLoopInstance(const LoopInstance* pLoopInstance) const;

	// 根据微连接点得到“回路实例”。
	LoopInstancePtr GetInstanceByMicroJointPt(const Point2D& microjointPt) const;

    // 根据几何轮廓得到轮廓实例。
    LoopInstanceListPtr GetLoopInstances_ByPatternLoop(LONGLONG iPatternLoopID) const;
	/************************************************************************/


	/************************************************************************/
	// 关于“回路起切特征”。

	// 得到所有回路实例的起切点。
	Point2DListPtr GetAllStartCutPts() const;

	// 得到所有“起切点”对应的“回路起切特征”。
	std::vector< std::pair<LoopStartCutDataPtr, Point2D> > GetAllStartCutPtPairs() const;

	// get all end cut points
	Point2DListPtr GetAllEndCutPts(BOOL bArrayLastInst = TRUE) const;
	/************************************************************************/


	/************************************************************************/
	// 关于“微连接特征”。

	// 得到所有回路实例上的微连接点。
	Point2DListPtr GetAllMicroJointPts() const;

	// 得到所有回路实例上“微连接点”对应的“微连接特征”。
	std::vector< std::pair<MicroJointPtr, Point2D> > GetAllMicroJointPtPairs() const;
	/************************************************************************/


	/************************************************************************/
	// 关于“角特征”。

	// 得到所有回路实例上的角特征点。
	Point2DListPtr GetAllCornerPts() const;

	// 得到所有回路实例上“角特征点”对应的“角特征”。
	std::vector< std::pair<ICornerPtr, Point2D> > GetAllCornerPtPairs() const;
	/************************************************************************/


    /************************************************************************/
    // 有关轮廓刀具

    // 得到轮廓刀具对象所在的轮廓实例。
    LoopInstancePtr GetLoopInstanceByLoopTool(const LoopToolData* pLoopToolData) const;

    // 得到所有轮廓刀具
    LoopToolDataListPtr GetLoopToolDataList() const;
    /************************************************************************/


	// 删除指定的回路实例。
	// notes: 
	//   1) we do not use the ID if loop inst because loop insts are create dynamically.
	//   2) we also return the index of the "will-deleted" loop, so we can restore it.
	int DeleteLoopInstance(LoopInstancePtr pLoopInstance);

	// 删除回路集合。
	void DeleteLoopInstances(const LoopInstanceList* pLoopInstList);

    // 根据几何轮廓删除轮廓实例。
    void DeleteLoopInstanceByPatternLoopID(LONGLONG iPatternLoopID);

	// get the rect box of loop instances.
	// 注:
	//   1) this function will use the cache data, so run more fast.
	//   2) 当没有轮廓时，返回尺寸为0的矩形。
	Rect2D GetRectBox() const;
};

END_CUTLEADER_NAMESPACE()
