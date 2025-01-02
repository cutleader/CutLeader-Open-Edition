#pragma once

#include "CutPartSequenceBase.h"
#include "Line2D.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(GridCutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// �����ֹ��򡱡�
class ClCutSequence_Export GridCutSequence : public CutPartSequenceBase
{
public:
	GridCutSequence(void);
	virtual ~GridCutSequence(void);

public: // implement IData interface.
	// ����¡���ʵ����
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
	// �õ���һ������·ʵ������
	LoopInstancePtr GetFstLoopInst();

	// �õ��ڲ��и��ߣ������и�˳��
	std::vector<Line2D> GetInnerCutLines() const;

	// �õ��ⲿ�и��ߣ������и�˳��
	std::vector<Line2D> GetOuterCutLines();

	// �õ����������/������
	std::pair<int, int> GetColumnRowNum() const;

	// ������ʵ���Ƿ��ڹ����С�
	bool IncludePartInst(PartInstancePtr pPartInstance) const;

private:
	// �õ�����Ŀ�ߡ�
	std::pair<double, double> GetPartSize() const;

	// �������ʵ������Ӿ��Ρ�
	Rect2D CalcOuterRect_of_partInsts() const;

private:
	// �����������и��һ�������
	PartInstanceListPtr m_pPartInsts;

	// �����ֵ���һ���ǿ�ʼ��
	RECT_CORNER m_iStartCorner;

	// �ڲ��и��������и
	bool m_bVertical_4_inner;

	// �������ʱ���и
	bool m_bCCW_4_outer;

	// �����ȡ�
	double m_dBeamWidth;
};

END_CUTLEADER_NAMESPACE()
