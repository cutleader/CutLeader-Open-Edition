#pragma once

#include "clDataImportExportCommon.h"
#include "IBusinessData.h"

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(ExpertLibItem)
DECLARE_CUTLEADER_CLASS(Material)
DECLARE_CUTLEADER_CLASS(Part)
DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(NestJob)
DECLARE_CUTLEADER_CLASS(ExportDataInfo)
DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(MachineItem)

BEGIN_CUTLEADER_NAMESPACE()

enum DxfDwgExportVersion;

// 数据导出管理器。
class ClDataImportExport_Export ExportManager
{
public:
	// 导出任务数据。
	static bool ExportTask(const TaskItem* pTaskItem, CString strFullFilePath);

	// 导出零件为dxf/dwg。
	static BOOL ExpPartAsDxfDwg(PartPtr pPart, CString strFilePath, DXF_VER_TYPE emDxfDwgExportVersion,
		DIM_UNIT_TYPE emExportUnitType, DIM_UNIT_TYPE emCurrentUnitType, BOOL bDwg);

	// 导出板材为dxf/dwg。
	static bool ExportSheetAsDxfDwg(const Sheet* pSheet, CString strFullFilePath, DXF_VER_TYPE emDxfDwgExportVersion,
		DIM_UNIT_TYPE emExportUnitType, DIM_UNIT_TYPE emCurrentUnitType, BOOL bIsExpShtBorder);

	// export "pExpertLibItem" to file.
	// notes:
	//   1) the MacDesc info also will be saved into the db.
	static BOOL ExportExpLibItem(ExpertLibItemPtr pExpertLibItem, CString strFilePath);

	// 导出材料。
	static BOOL ExportMat(MaterialPtr pMat, CString strFilePath);

	// 写入数据库的导出信息，需要调用者来提交事务。
	static void SaveDBExpInfo(DBConnectPtr pExpInfoConnect);

	// export part as part.
	static BOOL ExpPartAsPart(PartPtr pPart, CString strFilePath);

private:
	// 这几个函数负责把导出信息写入表格。
	static void SaveExpInfo(DBConnectPtr pExpInfoConnect, PartPtr pPart);
	// 这几个函数需要调用者来提交事务。
	static void SaveExpLibExpInfo(DBConnectPtr pExpInfoConnect); // 写入专家库的导出信息。
	static void SaveMatLibExpInfo(DBConnectPtr pExpInfoConnect); // 写入材料库的导出信息。
};

END_CUTLEADER_NAMESPACE()
