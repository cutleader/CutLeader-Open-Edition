#pragma once

#include "PatternPosition.h"
#include "ComCutSequenceBase.h"
#include "SheetComCutGroup.h"

DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(SheetComCutGroup)
DECLARE_CUTLEADER_CLASS(SheetComCutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// ��Ĺ��߹���
class SheetComCutSequence : public ComCutSequenceBase
{
public:
	SheetComCutSequence(void);
	SheetComCutSequence(SheetComCutGroupPtr pComCutGroup, LONGLONG iPartInstID);
	virtual ~SheetComCutSequence(void);

public: // implement IData interface.
	// ��¡���߹���
	// ע:
	//  1) ����¡�������顱����Ҫ�Ļ�����������
	virtual IDataPtr Clone() const override;

public: // implement interface ICutSequence.
	virtual CutSequenceType GetCutSequenceType() const override { return CutSequence_CommonCut; }
	virtual Point2D GetFirstCutPt() const override;
	virtual Point2D GetLastCutPt() const override; // ��������õ��ĵ���Ҫ����У�������ӹ��е�����ʱ�����ݹ����д��λ����Ϣ�������������ʵ��������Щƫ�

public: // implement interface ICutPartSequence.
	virtual BOOL LoopHasBeenCut(const LoopInstance* pLoopInstance) const override;
	virtual LoopInstanceListPtr GetLoopInstanceList() const override;
	virtual LoopInstancePtr GetLoopHasLead() const override;
	virtual PartInstanceListPtr GetPartInstances() const override;

public:
	virtual ComCutGroupBasePtr GetComCutGroupBase() const override { return m_pSheetComCutGroup; }
	virtual LONGLONG GetCommonEdgeInstanceID() const override { return m_iPartInstID; }

public: // get/set functions.
	void SetSheetComCutGroup(SheetComCutGroupPtr pComCutGroup) { m_pSheetComCutGroup = pComCutGroup; }
	SheetComCutGroupPtr GetSheetComCutGroup() const { return m_pSheetComCutGroup; }

	void SetPartInstID(LONGLONG iPartInstID) { m_iPartInstID = iPartInstID; }
	LONGLONG GetPartInstID() const { return m_iPartInstID; }

private:
	// ������߹������ڵġ������顱��
	SheetComCutGroupPtr m_pSheetComCutGroup;

	// ���ʵ��ID��
	LONGLONG m_iPartInstID;
};

END_CUTLEADER_NAMESPACE()
