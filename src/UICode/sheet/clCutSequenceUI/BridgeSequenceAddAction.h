#pragma once

#include "clCutSequenceUICommon.h"
#include "Point2D.h"
#include "Rect2D.h"
#include "SequenceEditActionBase.h"
#include <map>

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// 添加“桥接工序”的动作。
// 注：
//   1) 这个动作负责新建新的桥接工序，还可以向现有桥接工序上增加新的“桥接节点”。
//   2) 一个完整的动作会选择2个外围“回路实例”，切割顺序遵循选择顺序。
//   3) 当一个外围回路上有“(阵列)子程序工序”或者“共边工序”时，不能对之加桥接。
class BridgeSequenceAddAction : public SequenceEditActionBase
{
	typedef enum tagBdgeSeqAddState
	{
		NONE						= 0,
		FIRST_BDGE_PT_SELECTED		= 1,
		SECOND_BDGE_PT_SELECTED		= 2,
	} BDGESEQ_ADD_STATE;

public:
	BridgeSequenceAddAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	~BridgeSequenceAddAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_ADD_BDGESEQ; }
	virtual int GetActionName() const override { return IDS_SEQ_ADD_BDGE_SEQ; }
	virtual void UpdateForUndoRedo() override;
	virtual SnapInputPtr GetSnapInput() const override;

private:
	// 检查能否在这个“回路实例”上加桥接。
	// 注：
	//   1) 当一个外围回路上有“共边/连割工序”时，不能对之加桥接。
	BOOL LoopCanAddBdge(LoopInstancePtr pLoopInstance);

	// 在选中的2个“回路实例”间增加桥接。
	void AddBdgeCut();

private:
	BDGESEQ_ADD_STATE m_iState;

	/************************************************************************/	
	// 关于第一个“回路实例”的信息。

	// 回路实例。
	LoopInstancePtr m_pFstLoopInst;
	PatternListPtr m_pPatternsInFirstLoopInstance;

	// 桥接点。
	Point2D m_fstBdgePt;
	int m_iFstLoopNodeIdx;
	/************************************************************************/

	/************************************************************************/
	// 关于第二个“回路实例”的信息。

	// 回路实例。
	LoopInstancePtr m_pSndLoopInst;
	PatternListPtr m_pPatternsInSecondLoopInstance;

	// 桥接点。
	Point2D m_sndBdgePt;
	int m_iSndLoopNodeIdx;
	/************************************************************************/

	// 缓存所有的外围“回路实例”。
	LoopInstanceListPtr m_pOuterLoopInsts;

	// 缓存零件实例中各个回路实例的外接矩形。
	std::map<LONGLONG, std::pair<Rect2D, std::map<LONGLONG, Rect2D>>> m_partRectMap;
};

END_CUTLEADER_NAMESPACE()
