#pragma once

#include "PatternPosition.h"
#include "ComCutSequenceBase.h"
#include "CanvasComCutGroup.h"

DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(CanvasComCutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// 底图共边工序。
class CanvasComCutSequence : public ComCutSequenceBase
{
public:
	CanvasComCutSequence(void);
	CanvasComCutSequence(CanvasComCutGroupPtr pCanvasComCutGroup, LONGLONG iPatternLoopID);
	virtual ~CanvasComCutSequence(void);

public: // implement IData interface.
	// 克隆共边工序。
	// 注:
	//  1) 不克隆“共边组”，需要的话在外面做。
	virtual IDataPtr Clone() const override;

public: // implement interface ICutSequence.
	virtual CutSequenceType GetCutSequenceType() const override { return CutSequence_CanvasCommonCut; }
	virtual Point2D GetFirstCutPt() const override;
	virtual Point2D GetLastCutPt() const override; // 这个函数得到的点需要经过校正，当从过切点起切时，根据工序中存的位置信息计算出的坐标会和实际坐标有些偏差。

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
	// 这个共边工序所在的“共边组”。
	CanvasComCutGroupPtr m_pCanvasComCutGroup;

	// 几何轮廓ID。
	LONGLONG m_iPatternLoopID;
};

END_CUTLEADER_NAMESPACE()
