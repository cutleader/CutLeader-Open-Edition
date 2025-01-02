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
	// �����ݿ���ذ�ġ�
	// ע:
	//   1) ���ĳ������·������������Ч����������Ϊ�գ����Ժ�����Ҫ��������
	//   2) �������������˻�·����ô����½�û����֮��Ӧ�ġ���·�ӹ����ա������Ժ�����Ҫ��������
	//   3) after loading sheet, the cache data in sheet will be all updated.
	static SheetPtr LoadSheetFromDb(LONGLONG iSheetID, SHEETLOAD_TYPE iSheetLoadType);
	// �������ֻ�Ӱ�����ݿ��ļ����ذ�ģ������ͽ�������Ĳ�����
	static SheetPtr LoadSheet_onlyFromSheetDbFile(CString strSheetDbFilePath, LONGLONG iSheetID, const PartList* pPartList);

	// get the matLib/expLib info of sheet.
	static LONGLONG GetMaterialSizeID(DBConnectPtr pDBConnect);
	static LONGLONG GetParamConfigID(DBConnectPtr pDBConnect);

	// ����Ԥ�����ݡ�
	// ע��
	//   1) ���û��Ԥ�����ݵĻ������ؿ�ֵ��
	static ImgDataPtr LoadPreviewData(DBConnectPtr pDBConnect);

    // ��������������顣
    static void LoadPartGroups(DBConnectPtr pDBConnect, const PartPlacementList* pPartPlacements, PartGroupListPtr pPartGroups);

	// �õ�������ݿ��а�ĵ�������
	static unsigned int LoadSheetCountFromDb(DBConnectPtr pDBConnect);

private:
    // ��һ�ε���ʱ���붥�������ID�������ǵݹ���ذ���������������ڵ�����������飬������pPartGroups�С�
    // ע��
    // 1��ÿ��ݹ鶼���ض�ӦiPartGroupID����������顣
    static PartGroupPtr LoadPartGroup(DBConnectPtr pDBConnect, const PartPlacementList* pAllPartPlacements, PartGroupListPtr pPartGroups, LONGLONG iPartGroupID);
};

END_CUTLEADER_NAMESPACE()
