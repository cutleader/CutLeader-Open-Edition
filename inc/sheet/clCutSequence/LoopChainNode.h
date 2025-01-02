#pragma once

#include "ChainNodeBase.h"

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopChainNode)

BEGIN_CUTLEADER_NAMESPACE()

// “连割回路”节点。
class ClCutSequence_Export LoopChainNode : public ChainNodeBase
{
public:
	LoopChainNode(void);
	LoopChainNode(LoopInstancePtr pLoopInstance);
	virtual ~LoopChainNode(void);

public: // implement IData interface.
	virtual IDataPtr Clone() const override;

public: // implement interface IChainNode.
	virtual CHAIN_NODE_TYPE GetChainNodeType() const { return CHAIN_NODE_LOOP; }
	virtual double DistanceTo(const Point2D& pt) const;

public: // get/set functions.
	void SetLoopInst(LoopInstancePtr pLoopInstance) { m_pLoopInstance = pLoopInstance; }
	LoopInstancePtr GetLoopInstance() const { return m_pLoopInstance; }

private:
	// 外围回路。
	LoopInstancePtr m_pLoopInstance;
};

END_CUTLEADER_NAMESPACE()
