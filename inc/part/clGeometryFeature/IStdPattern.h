#pragma once

#include "IPattern.h"

BEGIN_CUTLEADER_NAMESPACE()

class IStdPattern;
typedef boost::shared_ptr<IStdPattern> StdPatternPtr;

// ����׼ͼ�Ρ��ӿڡ�
class IStdPattern : virtual public IPattern
{
public:
	// �õ���׼ͼ�εĶԳƵ㡣
	// ע��
	//   1) �ڡ���׼ͼ�Ρ�����ƫ�ƺ󡰶ԳƵ㡱���ֲ�����
	virtual Point2D GetSymmetryPt() const = 0;

	// ƫ�ơ���׼ͼ�Ρ���
	virtual BOOL OffsetStdPat(BOOL bInside, double dOffsetVal) = 0;
};

END_CUTLEADER_NAMESPACE()
