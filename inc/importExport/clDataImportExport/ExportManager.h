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

// ���ݵ�����������
class ClDataImportExport_Export ExportManager
{
public:
	// �����������ݡ�
	static bool ExportTask(const TaskItem* pTaskItem, CString strFullFilePath);

	// �������Ϊdxf/dwg��
	static BOOL ExpPartAsDxfDwg(PartPtr pPart, CString strFilePath, DXF_VER_TYPE emDxfDwgExportVersion,
		DIM_UNIT_TYPE emExportUnitType, DIM_UNIT_TYPE emCurrentUnitType, BOOL bDwg);

	// �������Ϊdxf/dwg��
	static bool ExportSheetAsDxfDwg(const Sheet* pSheet, CString strFullFilePath, DXF_VER_TYPE emDxfDwgExportVersion,
		DIM_UNIT_TYPE emExportUnitType, DIM_UNIT_TYPE emCurrentUnitType, BOOL bIsExpShtBorder);

	// export "pExpertLibItem" to file.
	// notes:
	//   1) the MacDesc info also will be saved into the db.
	static BOOL ExportExpLibItem(ExpertLibItemPtr pExpertLibItem, CString strFilePath);

	// �������ϡ�
	static BOOL ExportMat(MaterialPtr pMat, CString strFilePath);

	// д�����ݿ�ĵ�����Ϣ����Ҫ���������ύ����
	static void SaveDBExpInfo(DBConnectPtr pExpInfoConnect);

	// export part as part.
	static BOOL ExpPartAsPart(PartPtr pPart, CString strFilePath);

private:
	// �⼸����������ѵ�����Ϣд����
	static void SaveExpInfo(DBConnectPtr pExpInfoConnect, PartPtr pPart);
	// �⼸��������Ҫ���������ύ����
	static void SaveExpLibExpInfo(DBConnectPtr pExpInfoConnect); // д��ר�ҿ�ĵ�����Ϣ��
	static void SaveMatLibExpInfo(DBConnectPtr pExpInfoConnect); // д����Ͽ�ĵ�����Ϣ��
};

END_CUTLEADER_NAMESPACE()
