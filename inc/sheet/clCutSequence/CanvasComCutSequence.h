#pragma once

#include "PatternPosition.h"
#include "ComCutSequenceBase.h"
#include "CanvasComCutGroup.h"

DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(CanvasComCutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// ��ͼ���߹���
class CanvasComCutSequence : public ComCutSequenceBase
{
public:
	CanvasComCutSequence(void);
	CanvasComCutSequence(CanvasComCutGroupPtr pCanvasComCutGroup, LONGLONG iPatternLoopID);
	virtual ~CanvasComCutSequence(void);

public: // implement IData interface.
	// ��¡���߹���
	// ע:
	//  1) ����¡�������顱����Ҫ�Ļ�����������
	virtual IDataPtr Clone() const override;

public: // implement interface ICutSequence.
	virtual CutSequenceType GetCutSequenceType() const override { return CutSequence_CanvasCommonCut; }
	virtual Point2D GetFirstCutPt() const override;
	virtual Point2D GetLastCutPt() const override; // ��������õ��ĵ���Ҫ����У�������ӹ��е�����ʱ�����ݹ����д��λ����Ϣ�������������ʵ��������Щƫ�

public: // implement interface ICutPartSequence.
	virtual BOOL LoopHasBeenCut(const LoopInstance* pLoopInstance) const override;
	virtual LoopInstanceListPtr GetLoopInstanceList() const override;
	virtual LoopInstancePtr GetLoopHasLead() const override;
	virtual PartInstanceListPtr GetPartInstances() const override;

public:
	virtual ComCutGroupBasePtr GetComCutGroupBase() const override { return m_pCanvasComCutGroup; }
	virtual LONGLONG GetCommonEdgeInstanceID() const override { return m_iPatternLoopID; }

public: // get/set functions.
	void SetCanvasComCutGroup(CanvasComCutGroupPtr pCanvasComCutGroup) { m_pCanvasComCutGroup = pCanvasComCutGroup; }
	CanvasComCutGroupPtr GetCanvasComCutGroup() const { return m_pCanvasComCutGroup; }

	void SetPatternLoopID(LONGLONG iPatternLoopID) { m_iPatternLoopID = iPatternLoopID; }
	LONGLONG GetPatternLoopID() const { return m_iPatternLoopID; }

private:
	// ������߹������ڵġ������顱��
	CanvasComCutGroupPtr m_pCanvasComCutGroup;

	// ��������ID��
	LONGLONG m_iPatternLoopID;
};

END_CUTLEADER_NAMESPACE()
