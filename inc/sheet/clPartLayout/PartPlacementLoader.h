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

// “零件放置”加载器。
class ClPartLayout_Export PartPlacementLoader
{
public:
	// 加载板材中所有的“零件放置”对象。
	static void LoadPartPmts(DBConnectPtr pDBConnect, const PartList* pPartList, PartPlacementListPtr pPartPlacements);

	// how many part is involved in sheet.
	static int GetPartInstanceCount(DBConnectPtr pDBConnect, LONGLONG iSheetId, LONGLONG iPartID);

	// 加载板材中所有引用到的零件
	static PartListPtr LoadAllParts_onSheet(DBConnectPtr pDBConnect);
};

END_CUTLEADER_NAMESPACE()
