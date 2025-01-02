#pragma once

#include "NCRowList.h"
#include "clPostProcessorCommon.h"

DECLARE_CUTLEADER_CLASS(NCRow)
DECLARE_CUTLEADER_CLASS(NCRowList)

BEGIN_CUTLEADER_NAMESPACE()

// this is a can which keeps the NC code rows.
class ClPostProcessor_Export NCCan
{
public:
	NCCan(void);
	~NCCan(void);

public: // get/set functions.
	void SetNCRowList(NCRowListPtr pNCRowList) { m_pNCRowList = pNCRowList; }
	NCRowListPtr GetNCRowList() const { return m_pNCRowList; }

public: // other functions.
	void AppendNCRow(NCRowPtr pNCRow) { m_pNCRowList->push_back(pNCRow); }

private:
	// the NC rows within this can.
	NCRowListPtr m_pNCRowList;
};

END_CUTLEADER_NAMESPACE()
