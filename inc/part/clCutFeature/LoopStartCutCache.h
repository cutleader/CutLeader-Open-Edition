#pragma once

#include "clCutFeatureCommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(Triangle2D)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(RepeatData)

BEGIN_CUTLEADER_NAMESPACE()

// “回路起切特征”缓存信息。
class ClCutFeature_Export LoopStartCutCache
{
public:
	LoopStartCutCache(void);
	~LoopStartCutCache(void);

public: // get/set functions.
	void SetStartCutPt(const Point2D& startCutPt) { m_startCutPt = startCutPt; }
	const Point2D& GetStartCutPt() const { return m_startCutPt; }

	void SetPiercePt(const Point2D& piercePt) { m_piercePt = piercePt; }
	const Point2D& GetPiercePt() const { return m_piercePt; }

	void SetStartPtGeom(Triangle2DPtr pStartPtGeom) { m_pStartPtGeom = pStartPtGeom; }
	Triangle2DPtr GetStartPtGeom() const { return m_pStartPtGeom; }

	void SetLeadInBaseGeom(LineArc2DListPtr pGeom2DList) { m_pLeadInGeomList = pGeom2DList; }
	LineArc2DListPtr GetLeadInGeom() const { return m_pLeadInGeomList; }

	void SetLeadOutBaseGeom(LineArc2DListPtr pGeom2DList) { m_pLeadOutGeomList = pGeom2DList; }
	LineArc2DListPtr GetLeadOutGeom() const { return m_pLeadOutGeomList; }

	void SetRepeatData(RepeatDataPtr pRepeatData) { m_pRepeatData = pRepeatData; }
	RepeatDataPtr GetRepeatData() const { return m_pRepeatData; }

private:
	// the start cut point of the loop.
	// notes:
	//   1) for array loop, this is for first inst.
	Point2D m_startCutPt;

	// the pierce point of the loop.
	// notes:
	//   1) for array loop, this is for first inst.
	Point2D m_piercePt;


	/************************************************************************/
	// for base geoms of start cut and lead.

	// the triangles of start cut.
	Triangle2DPtr m_pStartPtGeom;

	// the base geom of lead.
	LineArc2DListPtr m_pLeadInGeomList;
	LineArc2DListPtr m_pLeadOutGeomList;

	// the repeat data.
	// notes:
	//   1) all repeat matrixes are based on the first inst.
	RepeatDataPtr m_pRepeatData;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
