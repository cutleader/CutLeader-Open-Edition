#pragma once

#include "Point2D.h"
#include "clPostProcessorCommon.h"

DECLARE_CUTLEADER_CLASS(NCRowList)
DECLARE_CUTLEADER_CLASS(NCSimulateItemList)

BEGIN_CUTLEADER_NAMESPACE()

// after generating NC code, we got this object.
// notes:
//   1) this class keeps NC codes and relative simulation data.
class ClPostProcessor_Export NCCodeData
{
public:
	NCCodeData(void);
	~NCCodeData(void);

public: // get/set functions.
	void SetNCRowList(NCRowListPtr pNCRowList) { m_pNCRowList = pNCRowList; }
	NCRowListPtr GetNCRowList() const { return m_pNCRowList; }

	void SetNCSimItemList(NCSimulateItemListPtr pNCSimItemList) { m_pNCSimItemList = pNCSimItemList; }
	NCSimulateItemListPtr GetNCSimItemList() const { return m_pNCSimItemList; }

private:
	// NC codes.
	NCRowListPtr m_pNCRowList;

	// the relative simulation data.
	NCSimulateItemListPtr m_pNCSimItemList;
};

END_CUTLEADER_NAMESPACE()
