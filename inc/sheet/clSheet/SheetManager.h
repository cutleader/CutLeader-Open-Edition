#pragma once

#include "clSheetCommon.h"
#include "DataProperty.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(DataItem)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(Part)

BEGIN_CUTLEADER_NAMESPACE()

// use this class to manager the sheet.
class ClSheet_Export SheetManager
{
public:
	// create a sheet.
	// notes:
	//   1) we should not consume matSize in this function, do it when save the sheet into database.
	static SheetPtr CreateNormalSheet(CString strSheetName, LONGLONG iMaterialSizeID, LONGLONG iParamConfigID);
	static SheetPtr CreateCanvasSheet(CString strSheetName, LONGLONG iMaterialSizeID, LONGLONG iParamConfigID);

	// delete a sheet.
	// notes:
	//   1) when delete a sheet, caller should check whether the sheet related to a nest job, if YES, "iSheetCount" will be set correctly.
	//   2) when deleting sheet, we should update mat db and main db.
	/* params:
	*  iSheetCount: the sheet count.
	*/
	static BOOL DeleteSheet(LONGLONG iSheetID, int iSheetCount = 1, BOOL bTransaction = TRUE);


	/************************************************************************/
	// 保存板材。

	// 新建板材数据库文件，保存新板材数据，这个函数只涉及板材数据库文件。
	static void SaveNewSheet2SheetDbFile(SheetPtr pSheet);

	// 更新现有板材到现有板材数据库文件，这个函数只更新板材数据库文件。
	static void SaveExistSheet2SheetDbFile(SheetPtr pSheet);
	/************************************************************************/


	/************************************************************************/
	// 克隆板材。

	// clone sheet.
	// notes:
	//   1) before clone the sheet, we should make sure material count is enough, you know, the cloned sheet will consume same material with old one. 
	//   2) we should not consume matSize in this function, do it when save the sheet into database.
	//   3) do not clone the part which is used by the sheet, sequence data will also be cloned. do not clone "Sheet::m_pSkeleMatSize".
	//   4) cache data of returned sheet is not updated.
	static SheetPtr CloneSheet(const Sheet* pSheet);

	// 深度克隆板材。
	// 注:
	//  1) 深度克隆板材之前需要先把零件克隆好，新的板材用了新的零件数据。<LONGLONG, PartPtr>是<老零件ID，新零件>。
	//  2) 外面需要考虑材料消耗问题。
	static SheetPtr CloneSheet_withNewParts(const Sheet* pOldSheet, const std::map<LONGLONG, PartPtr>& newParts);
	/************************************************************************/


	// 验证板材是否有效。
	// 注意:
	//   1) 出现以下一种情况，认为板材无效，需要修复。
	//      a) 没有对应的材料库。
	//      b) 没有对应的专家库。
	static BOOL VerifySheet(LONGLONG iSheetID);

	// 计算出板材的预览数据。
	// 注：
	//   1) 如果板材中没有图形，返回空值。
	static ImgDataPtr CalcPreviewData(const Sheet* pSheet);
};

END_CUTLEADER_NAMESPACE()
