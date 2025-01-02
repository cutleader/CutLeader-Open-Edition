#pragma once

#include "Line2D.h"
#include "CutNodeBase.h"

DECLARE_CUTLEADER_CLASS(CutLineNode)

BEGIN_CUTLEADER_NAMESPACE()

// “直线切割节点”
//  注：
//   1) if we will apply the offset of beam width, "m_pLine2D" should have been adjusted. any way, "m_lineSide" should be set correctly.
class ClCutSequence_Export CutLineNode : public CutNodeBase, public boost::enable_shared_from_this<CutLineNode>
{
public:
	CutLineNode(Line2DPtr pLine2D, LINE_SIDE lineSide, BOOL bShutDown, BOOL bCutLead, BOOL bIsCutPart);
	CutLineNode(const Point2D& startPt, const Point2D& endPt, LINE_SIDE lineSide, BOOL bShutDown, BOOL bCutLead, BOOL bIsCutPart);
	virtual ~CutLineNode(void);

public: // implement IData interface.
	virtual IDataPtr Clone() const override;

public: // implement ICutNode interface.
	virtual CutNodeType GetCutNodeType() const { return CutNode_CutLine; }
	virtual BOOL CanDraw() const { return TRUE; }
	virtual void Transform(Matrix2D mat) { m_pLine2D->Transform(mat); }

public: // set/get functions.
	void SetLine(Line2DPtr pLine2D) { m_pLine2D = pLine2D; }
	Line2DPtr GetLine() const { return m_pLine2D; }

	void SetLineSide(LINE_SIDE lineSide) { m_lineSide = lineSide; }
	LINE_SIDE GetLineSide() const { return m_lineSide; }

	BOOL IsShutDown() const { return m_bShutDown; }
	BOOL IsCutLead() const { return m_bCutLead; }
    BOOL IsCutPart() const { return m_bIsCutPart; }

private:
	Line2DPtr m_pLine2D;

	// the cut side.
	LINE_SIDE m_lineSide;

	// 是否关闭激光。
	BOOL m_bShutDown;

	// 是不是切割引入引出线
	BOOL m_bCutLead;

    // 这个切割节点是不是切割零件的。
    // 注：比如连割中连接轮廓的切割线就不是切割零件的，还有引线，预料切割线对应的切割节点，等等。
    BOOL m_bIsCutPart;
};

END_CUTLEADER_NAMESPACE()
