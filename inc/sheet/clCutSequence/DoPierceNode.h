#pragma once

#include "clCutFeatureCommon.h"
#include "Point2D.h"
#include "CutNodeBase.h"

DECLARE_CUTLEADER_CLASS(DoPierceNode)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export DoPierceNode : public CutNodeBase
{
public:
	DoPierceNode(void);
	DoPierceNode(const Point2D& pt, BOOL bStartFlyCut = FALSE);
	virtual ~DoPierceNode(void);

public:
	virtual IDataPtr Clone() const override;

public: // implement ICutNode interface.
	virtual CutNodeType GetCutNodeType() const { return CutNode_DoPierce; }
	virtual BOOL CanDraw() const { return FALSE; }
	virtual void Transform(Matrix2D mat) { m_point *= mat; }

public: // get/set functions.
	void SetDoPiercePt(Point2D point) { m_point = point; }
	Point2D GetDoPiercePt() const { return m_point; }

	void SetCutSide(PatternLoopCutSideType cutSide) { m_cutSide = cutSide; }
	PatternLoopCutSideType GetCutSide() const { return m_cutSide; }

	BOOL IsRealPierce() const { return m_bRealPierce; }
	void IsRealPierce(BOOL bRealPierce) { m_bRealPierce = bRealPierce; }

	BOOL IsDropHead() const { return m_bDropHead; }
	void IsDropHead(BOOL bDropHead) { m_bDropHead = bDropHead; }

	BOOL IsStartFlyCut() const { return m_bStartFlyCut; }
	void IsStartFlyCut(BOOL bStartFlyCut) { m_bStartFlyCut = bStartFlyCut; }

private:
	Point2D m_point;

	// the cut side of the loop.
	PatternLoopCutSideType m_cutSide;

	// �Ƿ�Ҫ��Ĵ̴���
	// ע��
	//  1) ��ʱҪ�̴���λ���Ѿ�����ˣ����Բ����ٴ̴��������̴��ڵ㻹������ġ�
	BOOL m_bRealPierce;

	// �Ƿ��ͷ��
	BOOL m_bDropHead;

	// �Ƿ�ʼ�����и
	BOOL m_bStartFlyCut;
};

END_CUTLEADER_NAMESPACE()
