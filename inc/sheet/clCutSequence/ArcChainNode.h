#pragma once

#include "ChainNodeBase.h"
#include "Point2D.h"
#include "Arc2D.h"

DECLARE_CUTLEADER_CLASS(ArcChainNode)

BEGIN_CUTLEADER_NAMESPACE()

// ¡°Á¬¸î»¡¡±½Úµã¡£
class ArcChainNode : public ChainNodeBase
{
public:
	ArcChainNode(void);
	ArcChainNode(const Arc2D& arc2D);
	virtual ~ArcChainNode(void);

public: // implement IData interface.
	virtual IDataPtr Clone() const override;

public: // implement interface IChainNode.
	virtual CHAIN_NODE_TYPE GetChainNodeType() const { return CHAIN_NODE_ARC; }
	virtual double DistanceTo(const Point2D& pt) const;

public: // get/set functions.
	void SetCutArc(const Arc2D& arc2D) { m_arc2D = arc2D; }
	const Arc2D& GetCutArc() const { return m_arc2D; }

private:
	Arc2D m_arc2D;
};

END_CUTLEADER_NAMESPACE()
