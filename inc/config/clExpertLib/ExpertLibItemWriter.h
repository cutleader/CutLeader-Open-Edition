#pragma once

#include "clExpertLibCommon.h"

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(ExpertLibItem)
DECLARE_CUTLEADER_CLASS(MachineItem)

BEGIN_CUTLEADER_NAMESPACE()

// this class write ExpertLibItem into database.
// notes:
//   1) use this class we can generate a new Expert Library item in database.
class ClExpertLib_Export ExpertLibItemWriter
{
public:
	/************************************************************************/
	// deal with the basic data of ExpertLibItem.

	// update the basic data of the ExpertLibItem.
	// notes:
	//   1) after call this, must call function "UpdateModifyDate".
	static void UpdateBasicData(DBConnectPtr pDBConnect, ExpertLibItemPtr pExpLibItem, BOOL bTransaction = TRUE);

	// update the modified data of the ExpertLibItem.
	static void UpdateModifyDate(DBConnectPtr pDBConnect, LONGLONG iExpLibItem, LONGLONG iModifyTimeStamp, BOOL bTransaction = TRUE);
	/************************************************************************/

	// save a new Expert Library item into database.
	// notes:
	//   1) user should update create/modify data before call this.
	static void SaveExpLibItem(DBConnectPtr pDBConnect, ExpertLibItemPtr pExpLibItem, BOOL bTransaction = TRUE);

	// delete an expert library item.
	static void DeleteExpLibItem(DBConnectPtr pDBConnect, LONGLONG iExpLibItemID, BOOL bTransaction = TRUE);
};

END_CUTLEADER_NAMESPACE()
