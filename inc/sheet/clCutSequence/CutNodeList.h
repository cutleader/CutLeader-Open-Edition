#pragma once

#include "clCutSequenceCommon.h"
#include "Matrix2D.h"
#include "CutNode.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ICutNode)
DECLARE_CUTLEADER_CLASS(CutNodeList)

BEGIN_CUTLEADER_NAMESPACE()

// “切割节点”集合。
class ClCutSequence_Export CutNodeList : public std::vector<ICutNodePtr>
{
public:
	CutNodeList(void);
	~CutNodeList(void);

public:
	/************************************************************************/
	// get some cut nodes.

	CutNodeListPtr GetCutNodeByType(CutNodeType cutNodeType);

	// 根据类型得到一个节点，调用者需要确认这种类型的节点只有一个，否则debug-warn。
	int GetOneCutNodeIndex_byType(CutNodeType iCutNodeType);

	// get the cut node by ID.
	ICutNodePtr GetCutNodeByID(LONGLONG iID);

	// get first/last cut node by cut node type.
	// notes:
	//   1) return the first cut node we meet, maybe cannot find one.
	ICutNodePtr GetFirstCutNode(CutNodeType cutNodeType);
	ICutNodePtr GetLastCutNode(CutNodeType cutNodeType);
	/************************************************************************/

	void Append(CutNodeListPtr pCutNodeList);

	CutNodeListPtr Clone();

	void Transform(Matrix2D mat);

	// 根据类型删除一个节点，调用者需要确认这种类型的节点只有一个，否则debug-warn。
	ICutNodePtr DeleteOneCutNode_byType(CutNodeType iCutNodeType);
};

END_CUTLEADER_NAMESPACE()
