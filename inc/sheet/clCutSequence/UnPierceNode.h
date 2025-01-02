#pragma once

#include "Point2D.h"
#include "CutNodeBase.h"

DECLARE_CUTLEADER_CLASS(UnPierceNode)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export UnPierceNode : public CutNodeBase
{
public:
	UnPierceNode(void);
	UnPierceNode(const Point2D& pt, BOOL bEndFlyCut = FALSE);
	virtual ~UnPierceNode(void);

public:
	virtual IDataPtr Clone() const override;

public: // implement ICutNode interface.
	virtual CutNodeType GetCutNodeType() const { return CutNode_UnPierce; }
	virtual BOOL CanDraw() const { return FALSE; }
	virtual void Transform(Matrix2D mat) { m_point *= mat; }

public: // get/set functions.
	void SetUnPiercePt(Point2D point) { m_point = point; }
	Point2D GetUnPiercePt() const { return m_point; }

	BOOL IsRaiseHead() const { return m_bRaiseHead; }
	void IsRaiseHead(BOOL bRaiseHead) { m_bRaiseHead = bRaiseHead; }

	BOOL IsEndFlyCut() const { return m_bEndFlyCut; }
	void IsEndFlyCut(BOOL bEndFlyCut) { m_bEndFlyCut = bEndFlyCut; }

private:
	Point2D m_point;

	// 是否抬头。
	BOOL m_bRaiseHead;

	// 是否结束飞行切割。
	BOOL m_bEndFlyCut;
};

END_CUTLEADER_NAMESPACE()
