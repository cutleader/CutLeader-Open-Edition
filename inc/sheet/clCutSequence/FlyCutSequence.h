#pragma once

#include "CutPartSequenceBase.h"
#include "Line2D.h"

DECLARE_CUTLEADER_CLASS(LineArc2DLoopList)
DECLARE_CUTLEADER_CLASS(FlyCutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// 阵列数据的表达，对数据的访问逻辑交给FlyCutSequence来做。
struct FlyCutGrid
{
	FlyCutGrid()
	{
		pAllHoles.reset();
		iFirstHoleIndex = INVALID_IDX;
		iSecondHoleIndex = INVALID_IDX;
		iHoleCount_of_firstDir = 0;
		iThirdHoleIndex = INVALID_IDX;
		iHoleCount_of_secondDir = 0;
	}

	// 所有孔。
	LineArc2DLoopListPtr pAllHoles;

	// 第一个孔。
	int iFirstHoleIndex;

	// 第二个孔，第一孔到第二孔确定了阵列的一个方向。
	int iSecondHoleIndex;
	int iHoleCount_of_firstDir; // 这个方向孔的个数。

	// 第三个孔，第一孔到第三孔确定了阵列的第二个方向。
	int iThirdHoleIndex;
	int iHoleCount_of_secondDir; // 这个方向孔的个数。
};

// “飞行切割工序”。
class ClCutSequence_Export FlyCutSequence : public CutPartSequenceBase
{
public:
	FlyCutSequence(void);
	FlyCutSequence(LoopInstanceListPtr pLoopInstances, RECT_CORNER iStartCorner, bool bVerticalCut, double dMax_turing_radius);
	virtual ~FlyCutSequence(void);

public: // implement IData interface.
	// 需要克隆回路实例。
	virtual IDataPtr Clone() const override;

public: // implement interface ICutSequence.
	virtual CutSequenceType GetCutSequenceType() const override { return CutSequence_FlyCut; }
	virtual Point2D GetFirstCutPt() const override;
	virtual Point2D GetLastCutPt() const override;

public: // implement interface ICutPartSequence.
	virtual BOOL LoopHasBeenCut(const LoopInstance* pLoopInstance) const override;
	virtual LoopInstanceListPtr GetLoopInstanceList() const override;
	virtual LoopInstancePtr GetLoopHasLead() const override;
	virtual PartInstanceListPtr GetPartInstances() const override;

public: // get/set functions.
	void SetLoopInsts(LoopInstanceListPtr pLoopInstances) { m_pLoopInsts = pLoopInstances; }

	void SetStartCorner(RECT_CORNER iStartCorner) { m_iStartCorner = iStartCorner; }
	RECT_CORNER GetStartCorner() const { return m_iStartCorner; }

	void IsVerticalCut(bool bVerticalCut) { m_bVerticalCut = bVerticalCut; }
	bool IsVerticalCut() const { return m_bVerticalCut; }

	double GetMax_turing_radius() const { return m_dMax_turing_radius; }
	void SetMax_turing_radius(double dMax_turing_radius) { m_dMax_turing_radius = dMax_turing_radius; }

public:
	// 得到飞行切割中的阵列信息。
	FlyCutGrid Build_FlyCutGrid() const;

	// 得到阵列的四个角孔。<孔的索引，孔的中心点>
	std::vector<std::pair<int, Point2D>> GetFourHoles_on_corner(const FlyCutGrid& flyCutGrid) const;

	// 得到起切孔/终止孔。
	int GetStartCut_hole(const FlyCutGrid& flyCutGrid) const;
	int GetEndCut_hole(const FlyCutGrid& flyCutGrid) const;

	// 计算从一个角孔发出去的两个方向。
	std::pair<Vector2D, Vector2D> GetTwoDirection_from_cornerHole(const FlyCutGrid& flyCutGrid, int iHoleIndex_of_cornerHole) const;

	// 得到这个方向上的阵列间距和孔的个数，<孔的个数, <方孔在这个方向上的长度或圆孔半径，孔中心点的间距>>。
	std::pair<int, std::pair<double,double>> GetHoleInfo_byDir(const FlyCutGrid& flyCutGrid, const Vector2D& dir) const;

	// 得到切割的U方向和snake方向。<U dir, snake dir>
	std::pair<Vector2D, Vector2D> GetDirection_U_and_Snake(const FlyCutGrid& flyCutGrid) const;

private:
	LoopInstanceListPtr m_pLoopInsts;

	// 从哪个角开始切。
	RECT_CORNER m_iStartCorner;

	// 选好起始角后，就按照竖直的方向开始切。
	bool m_bVerticalCut;

	// 最大转弯半径。
	double m_dMax_turing_radius;
};

END_CUTLEADER_NAMESPACE()
