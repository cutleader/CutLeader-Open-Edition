#pragma once

#include "PatternPosition.h"
#include "ComCutSequenceBase.h"
#include "SheetComCutGroup.h"

DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(SheetComCutGroup)
DECLARE_CUTLEADER_CLASS(SheetComCutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// 板材共边工序。
class SheetComCutSequence : public ComCutSequenceBase
{
public:
	SheetComCutSequence(void);
	SheetComCutSequence(SheetComCutGroupPtr pComCutGroup, LONGLONG iPartInstID);
	virtual ~SheetComCutSequence(void);

public: // implement IData interface.
	// 克隆共边工序。
	// 注:
	//  1) 不克隆“共边组”，需要的话在外面做。
	virtual IDataPtr Clone() const override;

public: // implement interface ICutSequence.
	virtual CutSequenceType GetCutSequenceType() const override { return CutSequence_CommonCut; }
	virtual Point2D GetFirstCutPt() const override;
	virtual Point2D GetLastCutPt() const override; // 这个函数得到的点需要经过校正，当从过切点起切时，根据工序中存的位置信息计算出的坐标会和实际坐标有些偏差。

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
	// 这个共边工序所在的“共边组”。
	SheetComCutGroupPtr m_pSheetComCutGroup;

	// 零件实例ID。
	LONGLONG m_iPartInstID;
};

END_CUTLEADER_NAMESPACE()
