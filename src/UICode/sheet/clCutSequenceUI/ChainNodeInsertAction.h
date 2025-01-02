#pragma once

#include "SequenceEditActionBase.h"
#include "Point2D.h"
#include "InsertPositionActionBar.h"

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(IChainNode)

BEGIN_CUTLEADER_NAMESPACE()

// 这个动作中可以插入“连割节点”。
class ChainNodeInsertAction : public SequenceEditActionBase
{
	typedef enum tagChainNodeInsertState
	{
		NONE						= 0,
		CHAIN_NODE_SELECTED			= 1,
	} CHAINNODE_INSERT_STATE;

public:
	ChainNodeInsertAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort,
		InsertPositionActionBar* pInsertPositionActionBar);
	~ChainNodeInsertAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_INSERT_CHAINNODE; }
	virtual int GetActionName() const override { return IDS_SEQ_INSERT_CHAIN_NODE; }
	virtual void UpdateForUndoRedo() override;
	virtual SnapInputPtr GetSnapInput() const override;

private:
	// 插入节点。
	void InsertChainNode();

private:
	CHAINNODE_INSERT_STATE m_iState;

	// 当前光标点。
	Point2D m_currentPt;

	// 被鼠标套住的“回路实例”。
	LoopInstancePtr m_pActiveLoopInstance;
	PatternListPtr m_pPatternsInActiveLoopInstance;

	// 新的起切点位置。
	BOOL m_bOldStartPt; // 还用老的起切点。
	Point2D m_newPt;
	int m_iNodeIndex;

	// 基准节点。
	// 注：
	//  1) 新插入的节点会被设为新的基准节点。
	IChainNodePtr m_pBaseChainNode;

	// 缓存所有的外围“回路实例”。
	LoopInstanceListPtr m_pOuterLoopInsts;

	InsertPositionActionBar* m_pInsertPositionActionBar;
};

END_CUTLEADER_NAMESPACE()
