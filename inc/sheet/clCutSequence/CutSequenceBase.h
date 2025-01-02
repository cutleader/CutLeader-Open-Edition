#pragma once

#include "ICutSequence.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

// ����Ļ��ࡣ
class CutSequenceBase : virtual public ICutSequence,
				   public ClData
{
public:
	CutSequenceBase(void);
	virtual ~CutSequenceBase(void);

public: // implement interface ICutSequence.
	virtual BOOL IsCutPart() const override { return TRUE; }
    virtual bool SequenceWillBeCut() const override { return true; }

public: // implement interface ICutPartSequence.
	virtual Point2DListPtr GetTracePtList() const { return m_pTracePtList; }
	virtual void SetTracePtList(Point2DListPtr pPtList) { m_pTracePtList = pPtList; }
	virtual void ResetTraceData();

	virtual BOOL DonotDropHeadAtBegin() const { return m_bDonotDropHeadAtBegin; }
	virtual void DonotDropHeadAtBegin(BOOL bDonotDropHeadAtBegin) { m_bDonotDropHeadAtBegin = bDonotDropHeadAtBegin; }

protected:
	// ����һ�������������ġ������ƶ��㡱��
	// ע��
	//   1) ��Ҫ������һ������ʱ�Ż��ʼ��������ݡ�
	Point2DListPtr m_pTracePtList;

	// �������Ϊ�ӹ��������ʱҪ��Ҫ�Ѽ���ͷ���¡�
	// ע��
	//   1) Ϊtrueʱ��һ�㶼�����ˡ�·�����á����Ӷ����ᾭ�������Ϸ����Ӷ�����̧�ż���ͷ��
	//   2) ������ֵΪ����ô��һ������ӹ�����ʱ��Ҫ�Ѽ���ͷ̧�𣬿�ʼ�ӹ��������ʱ��Ҫ�Ѽ���ͷ���¡�
	BOOL m_bDonotDropHeadAtBegin;
};

END_CUTLEADER_NAMESPACE()
