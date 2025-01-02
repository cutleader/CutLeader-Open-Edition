#pragma once

#include "Arc2D.h"
#include "CutNodeBase.h"

DECLARE_CUTLEADER_CLASS(CutArcNode)

BEGIN_CUTLEADER_NAMESPACE()

// the cut arc CutNode.
// notes:
//   1) if we will apply the offset of beam width, "m_arcPtr" should have been adjusted. any way, "m_side" should be set correctly.
class ClCutSequence_Export CutArcNode : public CutNodeBase,
							     public boost::enable_shared_from_this<CutArcNode>
{
public:
	CutArcNode(Arc2DPtr arcPtr, ARC_SIDE arcSide, BOOL bShutDown, BOOL bCutLead, BOOL bIsCutPart);
	CutArcNode(const Point2D& centerPt, double dRadius, double dStartAngle, double dSweepAngle, ARC_SIDE arcSide, BOOL bShutDown, BOOL bCutLead, BOOL bIsCutPart);
	virtual ~CutArcNode(void);

public: // implement IData interface.
	virtual IDataPtr Clone() const override;

public: // implement ICutNode interface.
	virtual CutNodeType GetCutNodeType() const { return CutNode_CutArc; }
	virtual BOOL CanDraw() const { return TRUE; }
	void Transform(Matrix2D mat) { m_arcPtr->Transform(mat); }

public: // set/get functions.
	void SetArc(Arc2DPtr arcPtr) { m_arcPtr = arcPtr; }
	Arc2DPtr GetArc() const { return m_arcPtr; }

	void SetArcSide(ARC_SIDE arcSide) { m_side = arcSide; }
	ARC_SIDE GetArcSide() const { return m_side; }

	BOOL IsShutDown() const { return m_bShutDown; }
	BOOL IsCutLead() const { return m_bCutLead; }
    BOOL IsCutPart() const { return m_bIsCutPart; }

private:
	Arc2DPtr m_arcPtr;
	ARC_SIDE m_side;

	// �Ƿ�رռ��⡣
	BOOL m_bShutDown;

	// �ǲ����и�����������
	BOOL m_bCutLead;

    // ����и�ڵ��ǲ����и�����ġ�
    // ע�����������������������и��߾Ͳ����и�����ģ��������ߣ�Ԥ���и��߶�Ӧ���и�ڵ㣬�ȵȡ�
    BOOL m_bIsCutPart;
};

END_CUTLEADER_NAMESPACE()
