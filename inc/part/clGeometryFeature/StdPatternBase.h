#pragma once

#include "IStdPattern.h"
#include "PatternBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// ����׼ͼ�Ρ��Ļ���ʵ�֡�
class StdPatternBase : public PatternBase, virtual public IStdPattern
{
public:
	StdPatternBase(void);
	virtual ~StdPatternBase(void);

public: // implement IPattern interface.
	virtual BOOL IsStdPattern() const override { return TRUE; }

public: // implement IStdPattern interface.
	virtual Point2D GetSymmetryPt() const { return m_basePt; }
};

END_CUTLEADER_NAMESPACE()
