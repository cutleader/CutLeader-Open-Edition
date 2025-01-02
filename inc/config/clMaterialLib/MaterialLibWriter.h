#pragma once

#include "clMaterialLibCommon.h"

DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(Material)
DECLARE_CUTLEADER_CLASS(MaterialThickness)
DECLARE_CUTLEADER_CLASS(MaterialList)
DECLARE_CUTLEADER_CLASS(IMaterialLibNode)
DECLARE_CUTLEADER_CLASS(MaterialLibNodeList)

BEGIN_CUTLEADER_NAMESPACE()

class ClMaterialLib_Export MaterialLibWriter
{
public:
	/************************************************************************/
	// ���ڡ����ϡ���

	// ������ϡ�
	static void SaveMat(DBConnectPtr pDBConnect, MaterialPtr pMaterial, BOOL bCreate, BOOL bSaveMatThick, BOOL bTransaction = TRUE);

	// ���¡����ϡ�����Ϣ��
	static void UpdateMat(DBConnectPtr pDBConnect, LONGLONG iMatID, CString strName, CString strComment, 
						  double dGravity, LONGLONG iModifyDate, BOOL bTransaction = TRUE);

	// ���²����޸�ʱ�䡣
	static void UpdateMatModData(DBConnectPtr pDBConnect, LONGLONG iMatID, LONGLONG iModifyTimeStamp, BOOL bTransaction = TRUE);

	// ɾ�����ϡ�
	static void DeleteMat(DBConnectPtr pDBConnect, LONGLONG iMatID, BOOL bTransaction = TRUE);

	// ���ÿɼ��Ĳ��ϡ�
	static void SetVisibleMat(DBConnectPtr pDBConnect, IntegerListPtr pVisibleMatList);
	/************************************************************************/

	/************************************************************************/
	// ���ڡ����Ϻ�ȡ���

	// ���桰���Ϻ�ȡ���
	static void SaveMatThick(DBConnectPtr pDBConnect, MaterialThicknessPtr pMatThick, BOOL bCreate, BOOL bSaveMatSize, BOOL bTransaction = TRUE);

	// ���¡����Ϻ�ȡ�����Ϣ��
	static void UpdateMatThick(DBConnectPtr pDBConnect, LONGLONG iMatThickID, CString strName, double dThick, CString strComment, BOOL bTransaction = TRUE);

	// ɾ�������Ϻ�ȡ���
	static void DeleteMatName(DBConnectPtr pDBConnect, LONGLONG iMatName, BOOL bTransaction = TRUE);
	/************************************************************************/

	/************************************************************************/
	// ���ڡ����ϳߴ硱��

	// ���桰���ϳߴ硱��
	/* params:
	*  bCreate: whether create new MatSize or save an existing MatSize object.
	*/
	static void SaveMatSize(DBConnectPtr pDBConnect, IMaterialSizePtr pMaterialSize, BOOL bCreate, BOOL bTransaction = TRUE);

	// ɾ�������ϳߴ硱��
	static void DeleteMatSize(DBConnectPtr pDBConnect, LONGLONG iMatSize, BOOL bTransaction = TRUE);

	// ����Ĭ�ϵĲ��ϳߴ硣
	static void SaveDflMatSize(DBConnectPtr pDBConnect, LONGLONG iMaterialSizeID);

	// �黹���ϵ����Ͽ⡣
	static void ReturnMatSize(DBConnectPtr pDBConnect, IMaterialSizePtr pMaterialSize, int iRetNum, BOOL bTransaction = TRUE);
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
