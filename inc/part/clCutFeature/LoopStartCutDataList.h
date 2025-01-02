#pragma once

#include "Point2D.h"
#include "ClData.h"
#include "clExpertLibCommon.h"
#include "ILoopStartCutPt.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(LeadList)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutFeature_Export LoopStartCutDataList : public std::vector<LoopStartCutDataPtr>,
											public ClData
{
public:
	LoopStartCutDataList(void);
	~LoopStartCutDataList(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;

	// clone a LoopStartCutDataList.
	IDataPtr Clone() const override;

public:
	/************************************************************************/
	// about getting loop start/end cut object.

	// get the loop pierce info by the loop id.
	LoopStartCutDataPtr GetItemByLoopID(LONGLONG iLoopId);

	// get start/end cut object.
	LoopStartCutDataPtr GetItemByID(LONGLONG iLoopStaEndCutID);
	/************************************************************************/

	/************************************************************************/
	// about delete start/end cut object.

	// delete the start/end cut data by loop ID.
	void DeleteByLoopID(LONGLONG iLoopID);

	// delete the start/end cut data by ID.
	void DeleteByIDList(IntegerListPtr pStaEndCutList);
	/************************************************************************/

	// get all start cut points.
	Point2DListPtr GetAllStartCutPts();

	// 得到所有“起切点”对应的“回路起切特征”。
	std::vector<std::pair<LoopStartCutDataPtr, Point2D>> GetAllStartCutPtPairs();

	// get all end cut points.
	Point2DListPtr GetAllEndCutPts();

	// 得到所有引入或引出线信息。
	LeadListPtr GetLeadList(LeadInOutType iLeadType);

	/************************************************************************/
	// 检查这些“回路起切特征”上是不是有某个相同的属性。

	// 检查所有的“切割开始和结束点”的类型是不是都是iSECutPtType。
	BOOL CheckSECutPtType(SECUT_PT_TYPE iSECutPtType) const;

	// 切出点的过切值是否都等于“dOverCut”。
	// 注:
	//   1) 只有当所有起切点都在闭合回路上时，才可以调用这个函数。
	BOOL HasSameOverCut(double dOverCut) const;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
