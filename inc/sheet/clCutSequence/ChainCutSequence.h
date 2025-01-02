#pragma once

#include "CutPartSequenceBase.h"


DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(IChainNode)
DECLARE_CUTLEADER_CLASS(ChainNodeList)
DECLARE_CUTLEADER_CLASS(ChainCutSequence)


BEGIN_CUTLEADER_NAMESPACE()


// “连割工序”。
class ClCutSequence_Export ChainCutSequence : public CutPartSequenceBase
{
public:
	ChainCutSequence(void);
	ChainCutSequence(ChainNodeListPtr pChainNodes);
	virtual ~ChainCutSequence(void);

public: // implement IData interface.
	virtual IDataPtr Clone() const override;

public: // implement interface ICutSequence.
	virtual CutSequenceType GetCutSequenceType() const override { return CutSequence_ChainCut; }
	virtual Point2D GetFirstCutPt() const override;
	virtual Point2D GetLastCutPt() const override;

public: // implement interface ICutPartSequence.
	virtual BOOL LoopHasBeenCut(const LoopInstance* pLoopInstance) const override;
	virtual LoopInstanceListPtr GetLoopInstanceList() const override;
	virtual LoopInstancePtr GetLoopHasLead() const override;
	virtual PartInstanceListPtr GetPartInstances() const override;

public: // get/set functions.
	void SetChainNodes(ChainNodeListPtr pChainNodes) { m_pChainNodes = pChainNodes; }
	ChainNodeListPtr GetChainNodes() const { return m_pChainNodes; }

    void SetResetPtList(Point2DListPtr pPtList_resetInstruction) { m_pPtList_resetInstruction = pPtList_resetInstruction; }
    Point2DListPtr GetResetPtList() const { return m_pPtList_resetInstruction; }

public:
	// 得到第一个“回路实例”。
	LoopInstancePtr GetFstLoopInst();

	// 计算这个连割切到的所有零件，注意不是零件实例。
	IntegerListPtr GetAllPartIDs() const;

	// 得到第一个和最后一个回路类型的节点。
	IChainNodePtr GetFirst_chainLoopNode() const;
	IChainNodePtr GetLast_chainLoopNode() const;

private:
	// “连割”节点。
	ChainNodeListPtr m_pChainNodes;

    // 复位点，目前是专门为一个客户定制的
    Point2DListPtr m_pPtList_resetInstruction;
};


END_CUTLEADER_NAMESPACE()
