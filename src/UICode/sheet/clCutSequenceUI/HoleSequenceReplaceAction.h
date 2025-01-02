#pragma once

#include "SequenceEditActionBase.h"
#include "Rect2D.h"
#include <map>

DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(ICutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// 拷贝“轮廓工序”的动作。
class HoleSequenceReplaceAction : public SequenceEditActionBase
{
public:
	HoleSequenceReplaceAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	~HoleSequenceReplaceAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_REPLACE_HOLE_SEQ; }
	virtual int GetActionName() const override { return IDS_SEQ_REPLACE_HOLE_SEQ; }
	virtual void UpdateForUndoRedo() override;

private:
	// 这些工序是不是靠在一起的。
	// 注：
	//  1) map中的第一个参数是工序在所有工序中的序号，这个整数应该依次增大1，这样才认为是靠在一起。
	BOOL IsSeqLinked(std::map<int, ICutSequencePtr> seqMap);

private:
	// 以这个零件为基准进行复制。
	PartInstancePtr m_pBasePartInst;

	// 缓存所有的“回路实例”。
	LoopInstanceListPtr m_pLoopInstanceList;

	// 缓存零件实例中各个回路实例的外接矩形。
	std::map<LONGLONG, std::pair<Rect2D, std::map<LONGLONG, Rect2D>>> m_partRectMap;
};

END_CUTLEADER_NAMESPACE()
