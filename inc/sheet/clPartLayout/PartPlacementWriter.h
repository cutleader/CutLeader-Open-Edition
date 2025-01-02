#pragma once

#include "clPartLayoutCommon.h"

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)
DECLARE_CUTLEADER_CLASS(PartPlacement)

BEGIN_CUTLEADER_NAMESPACE()

// ��������á���������
class ClPartLayout_Export PartPlacementWriter
{
public:
	// save the part placement into table.
	// ע��
	//   1) �������������ɾ�����ݡ�
	static BOOL SavePartPmt(DBConnectPtr pDBConnect, PartPlacementPtr pPartPlacement, LONGLONG iPartGroupID);

	// update the "cam data id" to db.
	// notes:
	//   1) this function do not need "m_iSheetId".
	static void UpdateCamData(DBConnectPtr pDBConnect, PartPlacementPtr pPartPlacement, BOOL bTransaction = TRUE);

	// save loop start/end cut info of part pmt.
	static void SaveLoopStaEndCutList(DBConnectPtr pDBConnect, LONGLONG iPartPmtId, LoopStartCutDataListPtr pLoopStaEndCutList, BOOL bTransaction = FALSE);
};

END_CUTLEADER_NAMESPACE()
