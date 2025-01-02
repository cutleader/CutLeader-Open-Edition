#pragma once

#include "clGeometryBaseCommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(NcItem)

BEGIN_CUTLEADER_NAMESPACE()

// nc项的类型。
enum NcItemType
{
	// 切割线。如G01行表示切割直线，G02、G03行表示切割顺/逆圆弧。
	NcItem_CutLine					= 0,

	// 快速移动线。如G00行表示快速移动线。
	NcItem_TraceLine				= 1,

	// 切割段序号。由g_CNS行表示，如“N426 G36 O$g_CNS$ D7”表示当前切割段是第7个切割段。一对儿pierce和unpierce之间的切割线称为一个切割段，
	// 需要注意的是，一个共边轮廓可能被分为两个切割段，还有，如果一个轮廓工序上有微连，不会把这个轮廓工序打断成多个切割段。
	NcItem_CutSegment_OrderNum		= 2,
};

// 一个nc项对应g代码中的一行。
// 注：
// 1) 目前只解析了部分nc代码行。
class NcItem
{
public:
	NcItem();
	~NcItem(void);

public:
	NcItemType GetNcItemType() const { return m_emNcItemType; }
	void SetNcItemType(NcItemType emNcItemType) { m_emNcItemType = emNcItemType; }

	ILineArc2DPtr GetLineArc2D() const { return m_pLineArc2D; }
	void SetLineArc2D(ILineArc2DPtr pLineArc2D) { m_pLineArc2D = pLineArc2D; }

	const Point2D& GetCurrentCutPt() const { return m_currentCutPt; }
	void SetCurrentCutPt(const Point2D& currentCutPt) { m_currentCutPt = currentCutPt; }

	int GetOrderNum() const { return m_iOrderNum; }
	void SetOrderNum(int iOrderNum) { m_iOrderNum = iOrderNum; }

private:
	// nc项的类型。
	NcItemType m_emNcItemType;

	// 切割路径或者快速移动线。
	ILineArc2DPtr m_pLineArc2D;

	// 用于切割段序号。
	Point2D m_currentCutPt; // 切割头当前位置。
	int m_iOrderNum; // 序号。
};

END_CUTLEADER_NAMESPACE()
