#pragma once

#include "SequenceEditActionBase.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(ChainCutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// 添加“连割工序”的动作。
// 注：
//  1) 在这个动作中会新建“连割工序”，也可能在现有“连割工序”后面添加“连割节点”。
class ChainCutSequenceAddAction : public SequenceEditActionBase
{
	typedef enum tagBdgeSeqAddState
	{
		NONE						= 0,
		CHAIN_SEQ_CREATED			= 1,
	} CHAINSEQ_ADD_STATE;

public:
	ChainCutSequenceAddAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	~ChainCutSequenceAddAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_ADD_CHAINSEQ; }
	virtual int GetActionName() const override { return IDS_SEQ_ADD_CHAIN_SEQ; }
	virtual void UpdateForUndoRedo() override;
	virtual SnapInputPtr GetSnapInput() const override;

private:
	// 检查这个“回路实例”能否加连割工序。
	// 注：
	//   1) 当这个回路上有“共边/桥接工序/连割”时，不能对之加连割。
	BOOL Check4CreateChain(LoopInstancePtr pLoopInstance);

	// 创建“连割工序”。
	void CreateChainSeq();

	// 在现有“连割工序”后面加节点。
	void AddChainNode();

private:
	CHAINSEQ_ADD_STATE m_iState;

	// 当前光标点。
	Point2D m_currentPt;

	// 被鼠标套住的“回路实例”。
	LoopInstancePtr m_pActiveLoopInstance;
	PatternListPtr m_pPatternsInActiveLoopInstance;

	// 新的起切点位置。
	BOOL m_bOldStartPt; // 还用老的起切点。
	Point2D m_newPt;
	int m_iNodeIndex;

	// 创建“连割工序”前选中的点。
	Point2DListPtr m_pCutPts;

	// 这个动作创建的连割工序。
	ChainCutSequencePtr m_pChainSeq;

	// 缓存所有的外围“回路实例”。
	LoopInstanceListPtr m_pOuterLoopInsts;
};

END_CUTLEADER_NAMESPACE()
