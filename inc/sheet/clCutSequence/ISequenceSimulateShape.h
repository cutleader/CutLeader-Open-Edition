#pragma once

#include "clCutSequenceCommon.h"
#include "Rect2D.h"

BEGIN_CUTLEADER_NAMESPACE()

enum SIM_SHAPE_TYPE
{
	SIM_SHAPE_CUT			= 0,
	SIM_SHAPE_TRACE			= 1,
};

// ģ�⡰�и�ڵ㡱����״��
// ע��
//   1) ��ģ����״���еĻ���ͼ���Ǵӡ��и�ڵ㡱��¡������
class ClCutSequence_Export ISequenceSimulateShape
{
public:
    virtual ~ISequenceSimulateShape() {}

public:
	virtual SIM_SHAPE_TYPE GetSimShapeType() const = 0;
	virtual void Transform(const Matrix2D& mat) = 0;
	virtual Rect2D GetRect() const = 0;
};

END_CUTLEADER_NAMESPACE()
