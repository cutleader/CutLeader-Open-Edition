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

// ���ݵ����������
class ClDataImportExport_Export ImportManager
{
public:
	/************************************************************************/
	// ���ڵ���dxf/dwg�ļ���

	// �õ�dxf�Ļ���ͼ�κ�dxf data��Ϣ��
	static std::pair<LineArc2DListPtr, DxfDataPtr> GetDxfDwgBaseGeomsAndDxfData(CString strFilePath, BOOL bDwg, const CString& strDefaultFontFileName);

	// ����dxf/dwg�ļ�Ϊ�������
	static PartPtr ImportDxfDwg2Part(CString strFilePath, const BuildPartCadDataOption& option, UINT iAssignLimit, const CString& strDefaultFontFileName);
	/************************************************************************/


	/************************************************************************/
	// ���ڵ���Cutleader����ļ���

	// get 2D line/arcs of .CLPrt file.
	static LineArc2DListPtr GetLineArc2Ds_ofClPart(CString strFilePath);

	// import a CLPrt file as a part object.
	// notes:
	//  1) the create/modify date will be set in this function.
	//  2) Ҫ�ǵ���ʧ�ܣ���������ڲ��ᱨ�����÷�Ҫ���һ�µ����豨��
	static PartPtr ImportClPart(CString strFilePath);
	/************************************************************************/


	// ��һ��CutLeader�����ļ����뵽ָ���Ĺ�����Ŀ¼������һ��Ŀ¼�ڵ㡣
	static FolderNodePtr ImportTaskItem2Folder_fromClTaskFile(CString strClTaskFilePath, FolderNodePtr pParentFolder);

	// import a TAOExpLib file as a ExpertLibItem object.
	// notes:
	//   1) driver version: when import expert library or driver, we must make sure the driver version is same with the current.
	static ExpertLibItemPtr ImportExpLibItem(CString strFilePath);

	// ������ϡ�
	static MaterialPtr ImportMat(CString strFilePath);

	// �õ����ݿ⵼����Ϣ��
	// ע:
	//   1) data in a DataProperty:
	//      1) ���ݿ�汾��
	static DataProperty GetExpDBProp(DBConnectPtr pExpInfoConnect);

private:
	// ��һ��CutLeader�����ļ�����Ϊ�������
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

	// �õ�ר�ҿ⵼����Ϣ��
	// notes:
	//   1) data in a DataProperty:
	//      1) ר�ҿ�汾��
	static DataProperty GetExpExpLibProp(DBConnectPtr pExpInfoConnect);

	// �õ����Ͽ⵼����Ϣ��
	// notes:
	//   1) data in a DataProperty:
	//      1) ���Ͽ�汾��
	static DataProperty GetExpMatLibProp(DBConnectPtr pExpInfoConnect);
};

END_CUTLEADER_NAMESPACE()
