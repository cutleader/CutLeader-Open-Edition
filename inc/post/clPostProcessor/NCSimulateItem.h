#pragma once

#include "clPostProcessorCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ICutNode)

BEGIN_CUTLEADER_NAMESPACE()

typedef boost::shared_ptr< std::vector<int> >	IntListPtr;

// the simulation info of NC code, 用于模拟NC代码的数据。
// notes:
//   1) all NC rows in "m_pNCRowIDList" will use "m_pCutNode" to simulate.
class ClPostProcessor_Export NCSimulateItem
{
public:
	NCSimulateItem(void);
	~NCSimulateItem(void);

public: // get/set functions.
	void SetCutNode(ICutNodePtr pCutNode) { m_pCutNode = pCutNode; }
	ICutNodePtr GetCutNode() const { return m_pCutNode; }

	void SetNCRowList(IntListPtr pNCRowIDList) { m_pNCRowIDList = pNCRowIDList; }
	IntListPtr GetNCRowList() const { return m_pNCRowIDList; }

public: // other functions.
	// whether this NCSim item can be used for "iNCRowID".
	BOOL FitNCRow(int iNCRowID) const;

private:
	// the cut node.
	ICutNodePtr m_pCutNode;

	// NC rows.
	// notes:
	//   1) caller manage the memory.
	IntListPtr m_pNCRowIDList;
};

END_CUTLEADER_NAMESPACE()
