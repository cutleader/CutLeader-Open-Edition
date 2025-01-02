#pragma once

#include "Line2D.h"
#include "CutNodeBase.h"

DECLARE_CUTLEADER_CLASS(TraceLineNode)

BEGIN_CUTLEADER_NAMESPACE()

// “快速移动切割节点”。
class ClCutSequence_Export TraceLineNode : public CutNodeBase,
							      public boost::enable_shared_from_this<TraceLineNode>
{
public:
	TraceLineNode(void);
	TraceLineNode(Line2DPtr pLine2D);
	virtual ~TraceLineNode(void);

public: // implement IData interface.
	virtual IDataPtr Clone() const override;

public: // implement ICutNode interface.
	virtual CutNodeType GetCutNodeType() const { return CutNode_TraceLine; }
	virtual BOOL CanDraw() const { return TRUE; }
	virtual void Transform(Matrix2D mat) { m_pLine2D->Transform(mat); }

public: // set/get functions.
	void SetLine(Line2DPtr pLine2D) { m_pLine2D = pLine2D; }
	Line2DPtr GetLine() const { return m_pLine2D; }

private:
	Line2DPtr m_pLine2D;
};

END_CUTLEADER_NAMESPACE()
