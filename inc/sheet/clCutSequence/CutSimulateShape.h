#pragma once

#include "SequenceSimulateShapeBase.h"


DECLARE_CUTLEADER_CLASS(ILineArc2D)

BEGIN_CUTLEADER_NAMESPACE()


// �����״ģ��һЩ���и�ڵ㡱����Щ���и�ڵ㡱ִ��ʵ���и
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
	// �и�·����
	// ע��
	//   1) �������͡��и�ڵ㡱�е�һ�¡�
	ILineArc2DPtr m_pGeom2D;

    // ���"ģ����״"�ǲ����и�����ġ�
    // ע�����������������������и��߾Ͳ����и�����ģ��������ߣ�Ԥ���и��߶�Ӧ���и�ڵ㣬�ȵȡ�
    BOOL m_bIsCutPart;
};

END_CUTLEADER_NAMESPACE()
