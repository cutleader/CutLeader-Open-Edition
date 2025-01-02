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
	// �ð��������������ݿ��ļ���
	static BOOL FillSheetDbFile(DBConnectPtr pDBConnect, SheetPtr pSheet, BOOL bTransaction = TRUE);

	// ���������ݿ��ļ��е����ݡ�
	static void DeleteAllDataInSheetDbFile(DBConnectPtr pDBConnect, BOOL bTransaction = TRUE);

	// only update "Sheet::m_pMatSize" into database.
	// notes:
	//   1) after call this, must call function "UpdateModifyDate".
	static void UpdateMatSize(DBConnectPtr pDBConnect, SheetPtr pSheet, BOOL bTransaction = TRUE);

	// �޸İ�ĵġ��������á���
	// notes:
	//   1) after call this, must call function "UpdateModifyDate".
	static void UpdateParamConfig(DBConnectPtr pDBConnect, SheetPtr pSheet, BOOL bTransaction = TRUE);

private:
	// ����Ԥ�����ݡ�
	// ע��
	//   1) �������ֱ�Ӱ�����д�����ݿ⡣
	static void SavePreviewData(DBConnectPtr pDBConnect, ImgDataPtr pImgData, BOOL bTransaction = TRUE);

    // �����������Ϣд�����ݿ⡣
    static void SavePartGroups2DB(DBConnectPtr pDBConnect, PartGroupListPtr pPartGroupList);
};

END_CUTLEADER_NAMESPACE()
