#pragma once

#include "SequenceSimulateShapeBase.h"


DECLARE_CUTLEADER_CLASS(ILineArc2D)

BEGIN_CUTLEADER_NAMESPACE()


// 这个形状模拟一些“切割节点”，这些“切割节点”执行实际切割。
class ClCutSequence_Export CutSimulateShape : public SequenceSimulateShapeBase
{
public:
	CutSimulateShape(ILineArc2DPtr pLineArc2D, const Rect2D& rectOfLineArc2D, BOOL bIsCutPart);
	virtual ~CutSimulateShape(void);

public: // implement interface ISequenceSimulateShape.
	virtual SIM_SHAPE_TYPE GetSimShapeType() const override { return SIM_SHAPE_CUT; }
	virtual void Transform(const Matrix2D& mat);

public: // get/set functions.
	const ILineArc2D* GetLineArc2D() const { return m_pGeom2D.get(); }
    BOOL IsCutPart() const { return m_bIsCutPart; }

private:
	// 切割路径。
	// 注：
	//   1) 这个对象和“切割节点”中的一致。
	ILineArc2DPtr m_pGeom2D;

    // 这个"模拟形状"是不是切割零件的。
    // 注：比如连割中连接轮廓的切割线就不是切割零件的，还有引线，预料切割线对应的切割节点，等等。
    BOOL m_bIsCutPart;
};

END_CUTLEADER_NAMESPACE()
