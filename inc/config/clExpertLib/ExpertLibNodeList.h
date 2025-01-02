#pragma once

#include "ExpertLibNode.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

class IExpertLibNode;
typedef boost::shared_ptr<IExpertLibNode> ExpLibNodePtr;

// the list of the expert library node.
class ClExpertLib_Export ExpertLibNodeList : public std::vector<ExpLibNodePtr>
{
public:
	ExpertLibNodeList(void);
	~ExpertLibNodeList(void);

public:
	// get the expert library node by the type.
	// notes:
	//   1) caller should make sure that the list only have one object 
	//      which the type is "type".
	ExpLibNodePtr GetExpLibNodeByType(EXPLIB_NODE_TYPE type);
};

END_CUTLEADER_NAMESPACE()
