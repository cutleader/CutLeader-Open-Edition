#pragma once

#include "boost/enable_shared_from_this.hpp"
#include "clPartLayoutCommon.h"
#include "ClData.h"
#include "Rect2D.h"
#include "SubjectBase.h"

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(RepeatData)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(Part)
DECLARE_CUTLEADER_CLASS(PartCamData)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// “零件放置”对象。
// 注意：
// 1) “零件放置”对象中有一套完整的“加工特征数据”。
// 2) 阵列的行偏移/列偏移：比如，行偏移的绝对值，是阵列中某个零件实例中心点到上侧或下侧零件实例中心点的距离。
// 3) 阵列的行间距/列间距：比如，行间距的绝对值，是阵列中某个零件实例外接矩形和上侧或下侧零件实例外接矩形之间的距离。
class ClPartLayout_Export PartPlacement : public ClData, public boost::enable_shared_from_this<PartPlacement>, public SubjectBase
{
public:
	PartPlacement(void);
	PartPlacement(PartPtr pPart, PartCamDataPtr pPartCamData, Matrix2D matrix = Matrix2D());
	PartPlacement(LONGLONG iPartPmtId, PartPtr pPart, PartCamDataPtr pPartCamData, const Matrix2D& matrix);
	~PartPlacement(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;

	// clone the part pmt.
	// notes:
	//   1) 不克隆零件项，需要在这个函数外面处理。
	virtual IDataPtr Clone() const override;

public: // get/set functions
	PartPtr GetPart() const { return m_pPart; }
	void SetPart(PartPtr pPart) { m_pPart = pPart; }

	PartCamDataPtr GetCamData() const { return m_pPartCamData; }
	void SetCamData(PartCamDataPtr pPartCamData) { m_pPartCamData = pPartCamData; }

	const Matrix2D& GetMatrix() const { return m_matrix; }
	void SetMatrix(const Matrix2D& mat) { m_matrix = mat; }

	BOOL IsGrid() const { return m_bGrid; }
	void IsGrid(BOOL bGrid) { m_bGrid = bGrid; }

	int GetRowCount() const { return m_iRowCount; }
	void SetRowCount(int iCount) { m_iRowCount = iCount; }

	int GetColCount() const { return m_iColCount; }
	void SetColCount(int iCount) { m_iColCount = iCount; }

	double GetSpacingX() const { return m_dSpacingX; }
	void SetSpacingX(double dSpacing) { m_dSpacingX = dSpacing; }

	double GetSpacingY() const { return m_dSpacingY; }
	void SetSpacingY(double dSpacing) { m_dSpacingY = dSpacing; }

	// get the base point of the part placement, actually this is the mouse-click point by user.
	// notes:
	//   1) the base point is the left-bottom point of the min-rect.
	Point2D GetBasePt() const;
	void SetBasePt(const Point2D& pt);

public: // other functions.
	// calc the base geoms of part pmt.
	// notes: 
	//  1) do not calc cut feature.
	//  2) if part pmt is a "huge grid", this function will cost some time.
	LineArc2DListPtr Calculate2DLineArcs() const;

	// get the distance between the part placement and the point.
	double DistanceTo(const Point2D& pt) const;

	// get the rectangle range of this part placement.
	// notes:
	//   1) do not include the cam features.
	//   2) 当零件中没有几何特征时，返回大小为0的矩形。
	Rect2D GetRectBox() const;

	// get the angle of the part placement.
	double GetAngle() const;

	// transform the part placement.
	void Transform(const Matrix2D& mat);


	/************************************************************************/
	// when move/rotate part pmt, call "CopyTo", when copy part pmt, call "Clone".
	
	// notes: we should not clone the pierce data.
	void CopyTo(PartPlacementPtr& pPartPlacement);
	void UpdateFrom(PartPlacementPtr pPartPlacement);
	/************************************************************************/


	/************************************************************************/
	// for part grid.

	// get the repeat data.
	// notes: 
	//   1) the matrix list do not have special order.
	//   2) the matrix list referred to the first instance.
	//   3) the count of matrix list equals (m_iRowCount*m_iColCount - 1)
	RepeatDataPtr GetRepeatData() const;

	// get the transform matrix of specified part instance.
	// notes:
	//   1) this matrix is NOT based on "m_matrix".
	Matrix2D GetInstMat(int iRow, int iCol) const;
	/************************************************************************/


	// whether the pattern loop exist.
	BOOL PatternLoopExist(LONGLONG iPatternLoopID) const;

	// get all repeat data.
	// notes: 
	//   1) the matrix list do not have special order.
	//   2) the matrix list do not referred to the first instance.
	//   3) the count of matrix list equals the count of part inst.
	RepeatDataPtr GetAllRepeatData() const;

	// 判断这个零件放置是不是在“pPartPlacement”前面。
	// 注：
	//  1) 用于排序中，当方向是向下时，越矮的排在前面。
	//  2) 用外接矩形来比较位置。
	BOOL BeforePartPmt(PartPlacementPtr pPartPlacement, POSITION_INFO iDir);

	// 得到所有几何轮廓的拓扑关系，定义这个函数是为了方便，就是调用几次get函数即可。
	const LoopTopologyItemList* GetLoopTopologyItems() const;
	LoopTopologyItemListPtr GetLoopTopologyItems_FW() const;

private:
	// reset some variables.
	void Reset();

private:
	// 零件信息
	PartPtr m_pPart;

	// 板材下的一套cam数据。
	PartCamDataPtr m_pPartCamData;

	// the transform matrix.
	// notes: 
	//   1) we adjust the part in this matrix, so the "[2][0], [2][1]" is NOT the base point.
	//   2) for Grid, we can use this Matrix to get the first instance.
	Matrix2D m_matrix;


	/************************************************************************/
	// these data are used for grid
	// notes:
	//   1) the grid is always X-axis and Y-axis direction.
	//   2) for grid, the angle in "m_matrix" will only affect the angle of each part inst, NOT the whole grid.

	// whether this part pmt is a grid.
	BOOL m_bGrid;

	// the column/row number.
	int	m_iRowCount;
	int	m_iColCount;

	// 阵列的行偏移/列偏移。
	double m_dSpacingX; // 列偏移。
	double m_dSpacingY; // 行偏移。
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
