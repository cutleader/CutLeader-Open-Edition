#pragma once

#include "clPartLayoutCommon.h"
#include "DataProperty.h"
#include <vector>
#include <map>

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartItem)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartList)

BEGIN_CUTLEADER_NAMESPACE()

// ��������á���������
class ClPartLayout_Export PartPlacementLoader
{
public:
	// ���ذ�������еġ�������á�����
	static void LoadPartPmts(DBConnectPtr pDBConnect, const PartList* pPartList, PartPlacementListPtr pPartPlacements);

	// how many part is involved in sheet.
	static int GetPartInstanceCount(DBConnectPtr pDBConnect, LONGLONG iSheetId, LONGLONG iPartID);

	// ���ذ�����������õ������
	static PartListPtr LoadAllParts_onSheet(DBConnectPtr pDBConnect);
};

END_CUTLEADER_NAMESPACE()
