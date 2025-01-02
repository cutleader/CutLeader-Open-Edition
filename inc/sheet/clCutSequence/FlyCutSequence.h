#pragma once

#include "CutPartSequenceBase.h"
#include "Line2D.h"

DECLARE_CUTLEADER_CLASS(LineArc2DLoopList)
DECLARE_CUTLEADER_CLASS(FlyCutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// �������ݵı������ݵķ����߼�����FlyCutSequence������
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

	// ���пס�
	LineArc2DLoopListPtr pAllHoles;

	// ��һ���ס�
	int iFirstHoleIndex;

	// �ڶ����ף���һ�׵��ڶ���ȷ�������е�һ������
	int iSecondHoleIndex;
	int iHoleCount_of_firstDir; // �������׵ĸ�����

	// �������ף���һ�׵�������ȷ�������еĵڶ�������
	int iThirdHoleIndex;
	int iHoleCount_of_secondDir; // �������׵ĸ�����
};

// �������и�򡱡�
class ClCutSequence_Export FlyCutSequence : public CutPartSequenceBase
{
public:
	FlyCutSequence(void);
	FlyCutSequence(LoopInstanceListPtr pLoopInstances, RECT_CORNER iStartCorner, bool bVerticalCut, double dMax_turing_radius);
	virtual ~FlyCutSequence(void);

public: // implement IData interface.
	// ��Ҫ��¡��·ʵ����
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
	// �õ������и��е�������Ϣ��
	FlyCutGrid Build_FlyCutGrid() const;

	// �õ����е��ĸ��ǿס�<�׵��������׵����ĵ�>
	std::vector<std::pair<int, Point2D>> GetFourHoles_on_corner(const FlyCutGrid& flyCutGrid) const;

	// �õ����п�/��ֹ�ס�
	int GetStartCut_hole(const FlyCutGrid& flyCutGrid) const;
	int GetEndCut_hole(const FlyCutGrid& flyCutGrid) const;

	// �����һ���ǿ׷���ȥ����������
	std::pair<Vector2D, Vector2D> GetTwoDirection_from_cornerHole(const FlyCutGrid& flyCutGrid, int iHoleIndex_of_cornerHole) const;

	// �õ���������ϵ����м��Ϳ׵ĸ�����<�׵ĸ���, <��������������ϵĳ��Ȼ�Բ�װ뾶�������ĵ�ļ��>>��
	std::pair<int, std::pair<double,double>> GetHoleInfo_byDir(const FlyCutGrid& flyCutGrid, const Vector2D& dir) const;

	// �õ��и��U�����snake����<U dir, snake dir>
	std::pair<Vector2D, Vector2D> GetDirection_U_and_Snake(const FlyCutGrid& flyCutGrid) const;

private:
	LoopInstanceListPtr m_pLoopInsts;

	// ���ĸ��ǿ�ʼ�С�
	RECT_CORNER m_iStartCorner;

	// ѡ����ʼ�Ǻ󣬾Ͱ�����ֱ�ķ���ʼ�С�
	bool m_bVerticalCut;

	// ���ת��뾶��
	double m_dMax_turing_radius;
};

END_CUTLEADER_NAMESPACE()
