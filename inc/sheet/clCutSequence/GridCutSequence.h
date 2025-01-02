#pragma once

#include "CutPartSequenceBase.h"
#include "Line2D.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(GridCutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// “田字工序”。
class ClCutSequence_Export GridCutSequence : public CutPartSequenceBase
{
public:
	GridCutSequence(void);
	virtual ~GridCutSequence(void);

public: // implement IData interface.
	// 不克隆零件实例。
	virtual IDataPtr Clone() const override;

public: // implement interface ICutSequence.
	virtual CutSequenceType GetCutSequenceType() const override { return CutSequence_GridCut; }
	virtual Point2D GetFirstCutPt() const override;
	virtual Point2D GetLastCutPt() const override;

public: // implement interface ICutPartSequence.
	virtual BOOL LoopHasBeenCut(const LoopInstance* pLoopInstance) const override;
	virtual LoopInstanceListPtr GetLoopInstanceList() const override;
	virtual LoopInstancePtr GetLoopHasLead() const override;
	virtual PartInstanceListPtr GetPartInstances() const override;

public: // get/set functions.
	void SetPartInsts(PartInstanceListPtr pPartInstances) { m_pPartInsts = pPartInstances; }

	void SetStartCorner(RECT_CORNER iStartCorner) { m_iStartCorner = iStartCorner; }
	RECT_CORNER GetStartCorner() const { return m_iStartCorner; }

	void IsVertical_4_inner(bool bVertical_4_inner) { m_bVertical_4_inner = bVertical_4_inner; }
	bool IsVertical_4_inner() const { return m_bVertical_4_inner; }

	void IsCCW_4_outer(bool bCCW_4_outer) { m_bCCW_4_outer = bCCW_4_outer; }
	bool IsCCW_4_outer() const { return m_bCCW_4_outer; }

	void SetBeamWidth(double dBeamWidth) { m_dBeamWidth = dBeamWidth; }
	double GetBeamWidth() const { return m_dBeamWidth; }

public:
	// 得到第一个“回路实例”。
	LoopInstancePtr GetFstLoopInst();

	// 得到内部切割线，按照切割顺序。
	std::vector<Line2D> GetInnerCutLines() const;

	// 得到外部切割线，按照切割顺序。
	std::vector<Line2D> GetOuterCutLines();

	// 得到零件的列数/行数。
	std::pair<int, int> GetColumnRowNum() const;

	// 这个零件实例是否在工序中。
	bool IncludePartInst(PartInstancePtr pPartInstance) const;

private:
	// 得到零件的宽高。
	std::pair<double, double> GetPartSize() const;

	// 计算零件实例的外接矩形。
	Rect2D CalcOuterRect_of_partInsts() const;

private:
	// 可以做田字切割的一组零件。
	PartInstanceListPtr m_pPartInsts;

	// 从田字的哪一个角开始。
	RECT_CORNER m_iStartCorner;

	// 内部切割先纵向切割。
	bool m_bVertical_4_inner;

	// 外框按照逆时针切割。
	bool m_bCCW_4_outer;

	// 刀缝宽度。
	double m_dBeamWidth;
};

END_CUTLEADER_NAMESPACE()
