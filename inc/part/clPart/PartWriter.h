#pragma once

#include "clPartCommon.h"

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(ExtPropValList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)

BEGIN_CUTLEADER_NAMESPACE()

// this class saves the Part into database.
// notes:
//   1) the "partData_trigger1" in "partData" table do not update "partCamData" table, so we should update it by code.
class ClPart_Export PartWriter
{
public:
	/************************************************************************/
	// deal with cad data of part.

	// save cad data.
	// notes:
	//   1) �������ֱ�Ӱ�����д�����ݿ⡣
	static void SaveCadData(DBConnectPtr pDBConnect, PartCadDataPtr pPartCadData, BOOL bTransaction = TRUE);

	// delete cad data.
	static void DeleteCadData(DBConnectPtr pDBConnect, BOOL bTransaction = TRUE);
	/************************************************************************/

	/************************************************************************/
	// deal with cam data of part.
	
	// save the cam data of the part.
	// notes:
	//   1) �������ֱ�Ӱ�����д�����ݿ⡣
	static void SaveCamData(DBConnectPtr pDBConnect, PartCamDataPtr pPartCamData, BOOL bTransaction = TRUE);

	// delete a cam data.
	static void DeleteCamData(DBConnectPtr pDBConnect, LONGLONG iPartCamDataID, BOOL bTransaction = TRUE);

	// ���¹������ݵġ��������á���
	static void UpdateCamDataConfig(DBConnectPtr pDBConnect, LONGLONG iCamDataID, LONGLONG iParamConfigID, BOOL bTransaction = TRUE);
	/************************************************************************/

	/************************************************************************/
	// ����Ԥ�����ݡ�

	// ����Ԥ�����ݡ�
	// ע��
	//   1) �������ֱ�Ӱ�����д�����ݿ⡣
	static void SavePreviewData(DBConnectPtr pDBConnect, ImgDataPtr pImgData, BOOL bTransaction = TRUE);

	// ɾ��Ԥ�����ݡ�
	static void DeletePreviewData(DBConnectPtr pDBConnect, BOOL bTransaction = TRUE);
	/************************************************************************/

	/************************************************************************/
	// ������չ���ԡ�

	// ������չ���ԡ�
	// ע��
	//   1) �������ֱ�Ӱ�����д�����ݿ⡣
	static void SaveExtProp(DBConnectPtr pDBConnect, ExtPropValListPtr pExtPropVals, BOOL bTransaction = TRUE);

	// ɾ����չ���ԡ�
	static void DeleteExtProp(DBConnectPtr pDBConnect, BOOL bTransaction);
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
