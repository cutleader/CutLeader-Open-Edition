#pragma once

#include "clDataImportExportCommon.h"
#include "DataProperty.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(IBusinessData)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(Part)
DECLARE_CUTLEADER_CLASS(ExpertLibItem)
DECLARE_CUTLEADER_CLASS(Material)
DECLARE_CUTLEADER_CLASS(DxfData)
DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DataItem)
DECLARE_CUTLEADER_CLASS(MachineItem)

BEGIN_CUTLEADER_NAMESPACE()

struct BuildPartCadDataOption;

// 数据导入管理器。
class ClDataImportExport_Export ImportManager
{
public:
	/************************************************************************/
	// 关于导入dxf/dwg文件。

	// 得到dxf的基本图形和dxf data信息。
	static std::pair<LineArc2DListPtr, DxfDataPtr> GetDxfDwgBaseGeomsAndDxfData(CString strFilePath, BOOL bDwg, const CString& strDefaultFontFileName);

	// 导入dxf/dwg文件为零件对象。
	static PartPtr ImportDxfDwg2Part(CString strFilePath, const BuildPartCadDataOption& option, UINT iAssignLimit, const CString& strDefaultFontFileName);
	/************************************************************************/


	/************************************************************************/
	// 关于导入Cutleader零件文件。

	// get 2D line/arcs of .CLPrt file.
	static LineArc2DListPtr GetLineArc2Ds_ofClPart(CString strFilePath);

	// import a CLPrt file as a part object.
	// notes:
	//  1) the create/modify date will be set in this function.
	//  2) 要是导入失败，这个函数内部会报错，调用方要检查一下但无需报错。
	static PartPtr ImportClPart(CString strFilePath);
	/************************************************************************/


	// 把一个CutLeader任务文件导入到指定的工作区目录，返回一个目录节点。
	static FolderNodePtr ImportTaskItem2Folder_fromClTaskFile(CString strClTaskFilePath, FolderNodePtr pParentFolder);

	// import a TAOExpLib file as a ExpertLibItem object.
	// notes:
	//   1) driver version: when import expert library or driver, we must make sure the driver version is same with the current.
	static ExpertLibItemPtr ImportExpLibItem(CString strFilePath);

	// 导入材料。
	static MaterialPtr ImportMat(CString strFilePath);

	// 得到数据库导出信息。
	// 注:
	//   1) data in a DataProperty:
	//      1) 数据库版本。
	static DataProperty GetExpDBProp(DBConnectPtr pExpInfoConnect);

private:
	// 把一个CutLeader任务文件导入为任务对象。
	static TaskItemPtr ImportTaskItem_fromClTaskFile(CString strClTaskFilePath);

	// get the export prop of the part.
	// notes:
	//   1) data in a DataProperty:
	//      1) part id
	//      2) part name
	//      3) part comment
	//      4) create date
	//      5) modify date
	//      6) part version.
	static DataProperty GetExpPartProp(DBConnectPtr pExpInfoConnect);

	// 得到专家库导出信息。
	// notes:
	//   1) data in a DataProperty:
	//      1) 专家库版本。
	static DataProperty GetExpExpLibProp(DBConnectPtr pExpInfoConnect);

	// 得到材料库导出信息。
	// notes:
	//   1) data in a DataProperty:
	//      1) 材料库版本。
	static DataProperty GetExpMatLibProp(DBConnectPtr pExpInfoConnect);
};

END_CUTLEADER_NAMESPACE()
