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
	// �����ġ�

	// �½�������ݿ��ļ��������°�����ݣ��������ֻ�漰������ݿ��ļ���
	static void SaveNewSheet2SheetDbFile(SheetPtr pSheet);

	// �������а�ĵ����а�����ݿ��ļ����������ֻ���°�����ݿ��ļ���
	static void SaveExistSheet2SheetDbFile(SheetPtr pSheet);
	/************************************************************************/


	/************************************************************************/
	// ��¡��ġ�

	// clone sheet.
	// notes:
	//   1) before clone the sheet, we should make sure material count is enough, you know, the cloned sheet will consume same material with old one. 
	//   2) we should not consume matSize in this function, do it when save the sheet into database.
	//   3) do not clone the part which is used by the sheet, sequence data will also be cloned. do not clone "Sheet::m_pSkeleMatSize".
	//   4) cache data of returned sheet is not updated.
	static SheetPtr CloneSheet(const Sheet* pSheet);

	// ��ȿ�¡��ġ�
	// ע:
	//  1) ��ȿ�¡���֮ǰ��Ҫ�Ȱ������¡�ã��µİ�������µ�������ݡ�<LONGLONG, PartPtr>��<�����ID�������>��
	//  2) ������Ҫ���ǲ����������⡣
	static SheetPtr CloneSheet_withNewParts(const Sheet* pOldSheet, const std::map<LONGLONG, PartPtr>& newParts);
	/************************************************************************/


	// ��֤����Ƿ���Ч��
	// ע��:
	//   1) ��������һ���������Ϊ�����Ч����Ҫ�޸���
	//      a) û�ж�Ӧ�Ĳ��Ͽ⡣
	//      b) û�ж�Ӧ��ר�ҿ⡣
	static BOOL VerifySheet(LONGLONG iSheetID);

	// �������ĵ�Ԥ�����ݡ�
	// ע��
	//   1) ��������û��ͼ�Σ����ؿ�ֵ��
	static ImgDataPtr CalcPreviewData(const Sheet* pSheet);
};

END_CUTLEADER_NAMESPACE()
