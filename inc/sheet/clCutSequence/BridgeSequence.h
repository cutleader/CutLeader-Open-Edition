#pragma once

#include "CutPartSequenceBase.h"
#include "BridgeNode.h"

DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(BridgeSequence)

BEGIN_CUTLEADER_NAMESPACE()

// “桥接工序”。
//  注：
//  1) “桥接工序”中每个轮廓的切割方向需要一样。
class ClCutSequence_Export BridgeSequence : public CutPartSequenceBase
{
public:
	BridgeSequence(void);
	virtual ~BridgeSequence(void);

public: // implement IData interface.
	// 克隆“桥接工序”。
	// 注：
	//  1) “回路实例”中的回路ID和“零件实例”还是旧的，调用者负责在外面更新。
	//  2) “桥接点”中的图形ID还是旧的，调用者负责在外面更新。
	virtual IDataPtr Clone() const override;

public: // implement interface ICutSequence.
	virtual CutSequenceType GetCutSequenceType() const override { return CutSequence_Bridge; }
	virtual Point2D GetFirstCutPt() const override;
	virtual Point2D GetLastCutPt() const override;

public: // implement interface ICutPartSequence.
	virtual BOOL LoopHasBeenCut(const LoopInstance* pLoopInstance) const override;
	virtual LoopInstanceListPtr GetLoopInstanceList() const override;
	virtual LoopInstancePtr GetLoopHasLead() const override { return m_pRootBdgeNode->GetLoopInstance(); }
	virtual PartInstanceListPtr GetPartInstances() const override;

public: // get/set functions.
	void SetRootBdgeNode(BridgeNodePtr pRootBdgeNode) { m_pRootBdgeNode = pRootBdgeNode; }
	BridgeNodePtr GetRootBdgeNode() const { return m_pRootBdgeNode; }

public:
	// 得到“回路实例”对应的“桥接节点”。
	BridgeNodePtr GetBdgeNodeByLoopInst(LoopInstancePtr pLoopInstance);

	// 这个“桥接节点”是否属于这个“桥接工序”。
	BOOL IncludeBdgeNode(BridgeNodePtr pBridgeNode);

	// 计算这个桥接切到的所有零件，注意不是零件实例。
	IntegerListPtr GetAllPartIDs() const;

private:
	// 根“桥接节点”。
	BridgeNodePtr m_pRootBdgeNode;
};

END_CUTLEADER_NAMESPACE()
