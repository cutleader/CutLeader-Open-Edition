#pragma once

#include "clSheetCommon.h"


DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(PartGroupList)


BEGIN_CUTLEADER_NAMESPACE()

// use this class to write sheet to database.
class ClSheet_Export SheetWriter
{
public:
	// 用板材数据填充板材数据库文件。
	static BOOL FillSheetDbFile(DBConnectPtr pDBConnect, SheetPtr pSheet, BOOL bTransaction = TRUE);

	// 清除板材数据库文件中的数据。
	static void DeleteAllDataInSheetDbFile(DBConnectPtr pDBConnect, BOOL bTransaction = TRUE);

	// only update "Sheet::m_pMatSize" into database.
	// notes:
	//   1) after call this, must call function "UpdateModifyDate".
	static void UpdateMatSize(DBConnectPtr pDBConnect, SheetPtr pSheet, BOOL bTransaction = TRUE);

	// 修改板材的“参数配置”。
	// notes:
	//   1) after call this, must call function "UpdateModifyDate".
	static void UpdateParamConfig(DBConnectPtr pDBConnect, SheetPtr pSheet, BOOL bTransaction = TRUE);

private:
	// 保存预览数据。
	// 注：
	//   1) 这个函数直接把数据写入数据库。
	static void SavePreviewData(DBConnectPtr pDBConnect, ImgDataPtr pImgData, BOOL bTransaction = TRUE);

    // 把零件分组信息写入数据库。
    static void SavePartGroups2DB(DBConnectPtr pDBConnect, PartGroupListPtr pPartGroupList);
};

END_CUTLEADER_NAMESPACE()
