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

	// 是否要真的刺穿。
	// 注：
	//  1) 有时要刺穿的位置已经被割开了，所以不用再刺穿，不过刺穿节点还是输出的。
	BOOL m_bRealPierce;

	// 是否低头。
	BOOL m_bDropHead;

	// 是否开始飞行切割。
	BOOL m_bStartFlyCut;
};

END_CUTLEADER_NAMESPACE()
