#pragma once

#include "clSheetCommon.h"
#include "DataProperty.h"
#include "DataPropItem.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(PartGroup)
DECLARE_CUTLEADER_CLASS(PartGroupList)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartList)


BEGIN_CUTLEADER_NAMESPACE()

// the sheet load type.
typedef enum tagSheetLoadType
{
	// basic data/MatSize/schemeID will be loaded.
	SHEETLOAD_BASIC				= 0,

	// load for preview, compare with "SHEETLOAD_BASIC" option, preview data will be loaded.
	SHEETLOAD_PREVIEW			= 1,

	// load all data.
	SHEETLOAD_ALL				= 2,
} SHEETLOAD_TYPE;

// use this class to load sheet from database.
class ClSheet_Export SheetLoader
{
public:
	// 从数据库加载板材。
	// 注:
	//   1) 如果某个“回路起切特征”无效，它将被设为空，所以后面需要完善它。
	//   2) 如果零件下新增了回路，那么板材下将没有与之对应的“回路加工工艺”，所以后面需要完善它。
	//   3) after loading sheet, the cache data in sheet will be all updated.
	static SheetPtr LoadSheetFromDb(LONGLONG iSheetID, SHEETLOAD_TYPE iSheetLoadType);
	// 这个函数只从板材数据库文件加载板材，所以送进来更多的参数。
	static SheetPtr LoadSheet_onlyFromSheetDbFile(CString strSheetDbFilePath, LONGLONG iSheetID, const PartList* pPartList);

	// get the matLib/expLib info of sheet.
	static LONGLONG GetMaterialSizeID(DBConnectPtr pDBConnect);
	static LONGLONG GetParamConfigID(DBConnectPtr pDBConnect);

	// 加载预览数据。
	// 注：
	//   1) 板材没有预览数据的话，返回空值。
	static ImgDataPtr LoadPreviewData(DBConnectPtr pDBConnect);

    // 加载所有零件分组。
    static void LoadPartGroups(DBConnectPtr pDBConnect, const PartPlacementList* pPartPlacements, PartGroupListPtr pPartGroups);

	// 得到板材数据库中板材的数量。
	static unsigned int LoadSheetCountFromDb(DBConnectPtr pDBConnect);

private:
    // 第一次调用时送入顶层零件组ID，含义是递归加载包括顶层零件组在内的所有子零件组，并放入pPartGroups中。
    // 注：
    // 1）每层递归都返回对应iPartGroupID参数的零件组。
    static PartGroupPtr LoadPartGroup(DBConnectPtr pDBConnect, const PartPlacementList* pAllPartPlacements, PartGroupListPtr pPartGroups, LONGLONG iPartGroupID);
};

END_CUTLEADER_NAMESPACE()
