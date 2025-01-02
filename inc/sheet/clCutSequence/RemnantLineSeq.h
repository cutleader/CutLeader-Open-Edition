#pragma once

#include "CutSequenceBase.h"

DECLARE_CUTLEADER_CLASS(Line2DList)
DECLARE_CUTLEADER_CLASS(RemnantLineSeq)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export RemnantLineSeq : public CutSequenceBase
{
public:
	RemnantLineSeq(void);
	virtual ~RemnantLineSeq(void);

public: // implement IData interface.
	virtual IDataPtr Clone() const override;

public: // implement interface ICutSequence.
	virtual CutSequenceType GetCutSequenceType() const override { return CutSequence_RemnantCutLine; }
	virtual Point2D GetFirstCutPt() const override;
	virtual Point2D GetLastCutPt() const override;
	virtual BOOL IsCutPart() const override { return FALSE; }

public:
	Line2DListPtr GetCutLines() const { return m_pCutLines; }
	void SetCutLines(Line2DListPtr pCutLines) { m_pCutLines = pCutLines; }

private:
	// ÇÐ¸îÏß
	Line2DListPtr m_pCutLines;
};

END_CUTLEADER_NAMESPACE()
