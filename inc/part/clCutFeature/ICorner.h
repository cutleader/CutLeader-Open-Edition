#pragma once

#include "clExpertLibCommon.h"
#include "clCutFeatureCommon.h"
#include "IData.h"

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)

BEGIN_CUTLEADER_NAMESPACE()

// �����������ӿڡ�
// ע��
//    1) we can only auto assign corner to close polygon loop, BUT can manually assign to open polygon loop.
//    2) ��������û���и�������
//    3) ����ID�м�¼�ˡ���·��������ID��
class ClCutFeature_Export ICorner : virtual public IData
{
public:
    virtual ~ICorner() {}

public:
	virtual CornerType GetCornerType() const = 0;

	virtual LONGLONG GetFirstPatternID() const = 0;
	virtual void SetFirstPatternID(LONGLONG iPatternID) = 0;

	virtual LONGLONG GetSecondPatternID() const = 0;
	virtual void SetSecondPatternID(LONGLONG iPatternID) = 0;

	// ������������Ķ�άֱ�ߺ�Բ��������Ҫʵ�ʼӹ��ĵ�·������ʾ���Եġ�
    virtual bool HasCutPath() const = 0; // �������Ƿ���ʵ�ʼӹ���·��
	virtual LineArc2DListPtr Calculate2DLineArcs(const IPatternLoop* pPatternLoop, BOOL bFollowLoopDirection) const = 0;

    // ����������Ƿ��С�������ֵ������򻷽������а뾶ֵ������ȴ�����������ֵ���á�
    virtual bool HasCornerValue() const = 0;
};

END_CUTLEADER_NAMESPACE()
