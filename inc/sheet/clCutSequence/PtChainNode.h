#pragma once

#include "ChainNodeBase.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(PtChainNode)

BEGIN_CUTLEADER_NAMESPACE()

// “点”类型的连割节点。
class ClCutSequence_Export PtChainNode : public ChainNodeBase
{
public:
	PtChainNode(void);
	PtChainNode(const Point2D& pt);
	virtual ~PtChainNode(void);

public: // implement IData interface.
	virtual IDataPtr Clone() const override;

public: // implement interface IChainNode.
	virtual CHAIN_NODE_TYPE GetChainNodeType() const { return CHAIN_NODE_PT; }
	virtual double DistanceTo(const Point2D& pt) const;

public: // get/set functions.
	void SetCutPt(const Point2D& pt) { m_cutPt = pt; }
	const Point2D& GetCutPt() const { return m_cutPt; }

private:
	// 连割点。
	Point2D m_cutPt;
};

END_CUTLEADER_NAMESPACE()
