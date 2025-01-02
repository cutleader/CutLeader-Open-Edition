#pragma once

#include "clCutSequenceCommon.h"
#include "ClData.h"
#include "IChainNode.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export ChainNodeBase : public IChainNode, public ClData
{
public:
	ChainNodeBase(void);
	virtual ~ChainNodeBase(void);
};

END_CUTLEADER_NAMESPACE()
