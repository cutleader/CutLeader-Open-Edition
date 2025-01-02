#pragma once

#include "clPartLayoutCommon.h"

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(PartInstance)

BEGIN_CUTLEADER_NAMESPACE()

// use this class to load part instance from database.
class ClPartLayout_Export PartInstanceLoader
{
public:
	// load all part inst in the sheet.
	/* params:
	*  pAllPartPmt: all part pmts.
	*  pPartInstances: the "out" param.
	*/
	static void LoadPartInstList(DBConnectPtr pDBConnect, PartPlacementListPtr pAllPartPmt, PartInstanceListPtr pPartInstances);

	// load part inst by ID.
	/* params:
	*  pAllPartPmt: all part pmts.
	*/
	static PartInstancePtr LoadPartInst(DBConnectPtr pDBConnect, LONGLONG iPartInstID, PartPlacementListPtr pAllPartPmt);

	// load part insts by part pmt.
	// notes: 
	//   1) if part pmt is not grid, only one part inst will be returned.
	static PartInstanceListPtr LoadPartInst(DBConnectPtr pDBConnect, PartPlacementPtr pPartPlacement);
};

END_CUTLEADER_NAMESPACE()
