#pragma once

#include "PatternPosition.h"
#include "CutPartSequenceBase.h"


DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(ComCutGroupBase)
DECLARE_CUTLEADER_CLASS(ComCutSequenceBase)

// ���߱ȹ�����Сʱ���������������
#define OVERCUT_FACTOR	1/3.0

BEGIN_CUTLEADER_NAMESPACE()

// ���߹�����ࡣ
class ComCutSequenceBase : public CutPartSequenceBase
{
public:
	ComCutSequenceBase(void);
	virtual ~ComCutSequenceBase(void);

public: // get/set functions.
	void IsStartOnEdge(BOOL bStartOnEdge) { m_bStartOnEdge = bStartOnEdge; }
	BOOL IsStartOnEdge() const { return m_bStartOnEdge; }

	void SetStartPatID(LONGLONG iStartPatID) { m_iStartPatID = iStartPatID; }
	LONGLONG GetStartPatID() const { return m_iStartPatID; }

	void SetStartPatPos(const PatternPosition& startPatPos) { m_startPatPos = startPatPos; }
	const PatternPosition& GetStartPatPos() const { return m_startPatPos; }

	void SetEndPatID(LONGLONG iEndPatID) { m_iEndPatID = iEndPatID; }
	LONGLONG GetEndPatID() const { return m_iEndPatID; }

	void SetEndPatPos(const PatternPosition& endPatPos) { m_endPatPos = endPatPos; }
	const PatternPosition& GetEndPatPos() const { return m_endPatPos; }

public:
	virtual ComCutGroupBasePtr GetComCutGroupBase() const = 0;
	virtual LONGLONG GetCommonEdgeInstanceID() const = 0; // �õ�����ʵ��ID���ο�����EdgeLoop��ע�͡�

protected:
	/************************************************************************/
	// �ӡ��߶���������ʱ�õ��ı�����

	// �Ƿ�ӡ��߶������µ���
	BOOL m_bStartOnEdge;

	// �����λ�á�
	LONGLONG m_iStartPatID;
	PatternPosition m_startPatPos;

	// �г���λ�á�
	LONGLONG m_iEndPatID;
	PatternPosition m_endPatPos;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
