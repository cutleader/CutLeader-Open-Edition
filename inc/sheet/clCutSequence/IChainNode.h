#pragma once

#include "clCutSequenceCommon.h"
#include "IData.h"
#include "Point2D.h"


BEGIN_CUTLEADER_NAMESPACE()


// ����ڵ������
typedef enum tagChainNodeType
{
	CHAIN_NODE_PT		= 0,
	CHAIN_NODE_LOOP		= 1,
	CHAIN_NODE_ARC		= 2,
} CHAIN_NODE_TYPE;


// ����ڵ㡣
class IChainNode : virtual public IData
{
public:
    virtual ~IChainNode() {}

public:
	virtual CHAIN_NODE_TYPE GetChainNodeType() const = 0;

	// �õ��㵽�ڵ�ľ��롣
	virtual double DistanceTo(const Point2D& pt) const = 0;
};


END_CUTLEADER_NAMESPACE()
