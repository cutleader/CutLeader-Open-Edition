#pragma once

#include "clPartLayoutCommon.h"
#include "Point2D.h"
#include "Rect2D.h"
#include "ClData.h"
#include "SubjectBase.h"
#include <vector>
#include <map>

DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(LoopCornerDataList)
DECLARE_CUTLEADER_CLASS(LoopMicroJointList)
DECLARE_CUTLEADER_CLASS(PartItemList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartList)

BEGIN_CUTLEADER_NAMESPACE()

// “零件放置”集合。
class ClPartLayout_Export PartPlacementList : public std::vector<PartPlacementPtr>, public ClData, public SubjectBase
{
public:
	PartPlacementList(void);
	virtual ~PartPlacementList(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;

	// clone the part placement list.
	// notes:
	//   1) do not clone part BUT clone loop pierce data.
	//   2) do not clone the draw data, user should update it outside.
	virtual IDataPtr Clone() const override;

public:
	/************************************************************************/
	// about finding the part placement

	// get the part placement by id
	PartPlacementPtr GetPartPlacementByID(LONGLONG iId) const;

	// get the part placement which is closest to "pt".
	PartPlacementPtr GetPartPlacementClosestTo(const Point2D& pt) const;
	PartPlacementPtr GetPartPlacementClosestTo(const Point2D& pt, const std::map<LONGLONG, Rect2D>& m_rects_of_partPmt) const;

	// get the part placements within "rect".
	PartPlacementListPtr GetPartPlacementInRect(const Rect2D& rect) const;
    PartPlacementListPtr GetPartPlacementInRect(const Rect2D& rect, const std::map<LONGLONG, Rect2D>& m_rects_of_partPmt) const;

	// get the part placements which are from "iPartID".
	PartPlacementListPtr GetPartPlacementByPart(LONGLONG iPartID) const;

	// get index of "pPartPlacement".
	int GetPartPlacementIndex(const PartPlacement* pPartPlacement) const;
	/************************************************************************/


	/************************************************************************/
	// 关于得到零件的信息。

	// 得到所有引用到的零件的“CAD数据”。
	// 注：
	//  1) 得到零件ID和CAD数据的对应，零件ID不重复。
	std::map<LONGLONG, PartCadDataPtr> GetAllPartCadData() const;

	// 得到板材中的零件项。
	PartListPtr GetPartList() const;

	// 得到“零件放置”集合中引用到的所有不同零件。
	IntegerListPtr GetPartIDList() const;

	// 得到零件实例的个数。
	int GetPartInstanceCount(LONGLONG iPartID) const;
	int GetPartInstanceCount() const;
	/************************************************************************/


	/************************************************************************/
	// 关于删除零件放置。

	// 删除一个“零件放置”。
	void RemovePartPlacement(const PartPlacement* partPmtPtr);

	// 删除多个“零件放置”。
	void DeletePartPlacementList(const PartPlacementList* pPartPlacements);

	// 根据零件ID删除零件放置。
	void DeletePartPmtByPartID(LONGLONG iPartID);
	/************************************************************************/


	// get the base geometry.
	LineArc2DListPtr Calculate2DLineArcs() const;

	// 得到所有“回路微连接特征”。
	LoopMicroJointListPtr GetAllLoopMicroJointList() const;

	// 得到所有“回路角特征”。
	LoopCornerDataListPtr GetAllLoopCornerList() const;

	// 得到“零件放置”集合的外围矩形。
	// 注：
	//  1) 不包含加工工艺。
	Rect2D GetRectBox() const;

	void AppendPartPmts(const PartPlacementList* pPartPlacements) { this->insert(this->end(), pPartPlacements->begin(), pPartPlacements->end()); }

	// 变换零件放置集合。
	void Transform(const Matrix2D& transformMatrix);
};

END_CUTLEADER_NAMESPACE()
