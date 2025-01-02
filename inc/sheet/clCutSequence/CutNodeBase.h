#pragma once

#include "clCutSequenceCommon.h"
#include "ClData.h"
#include "CutNode.h"

BEGIN_CUTLEADER_NAMESPACE()

// ���и�ڵ㡱�ӿڵĻ���ʵ�֡�
class ClCutSequence_Export CutNodeBase : virtual public ICutNode,
								  public ClData
{
public:
	CutNodeBase(void);
	virtual ~CutNodeBase(void);

public: // implement ICutNode interface
	virtual BOOL CanDraw() const { return TRUE; }
	virtual void Transform(Matrix2D mat) {}
};

END_CUTLEADER_NAMESPACE()
