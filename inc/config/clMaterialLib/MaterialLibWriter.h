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
	// 关于“材料”。

	// 保存材料。
	static void SaveMat(DBConnectPtr pDBConnect, MaterialPtr pMaterial, BOOL bCreate, BOOL bSaveMatThick, BOOL bTransaction = TRUE);

	// 更新“材料”的信息。
	static void UpdateMat(DBConnectPtr pDBConnect, LONGLONG iMatID, CString strName, CString strComment, 
						  double dGravity, LONGLONG iModifyDate, BOOL bTransaction = TRUE);

	// 更新材料修改时间。
	static void UpdateMatModData(DBConnectPtr pDBConnect, LONGLONG iMatID, LONGLONG iModifyTimeStamp, BOOL bTransaction = TRUE);

	// 删除材料。
	static void DeleteMat(DBConnectPtr pDBConnect, LONGLONG iMatID, BOOL bTransaction = TRUE);

	// 设置可见的材料。
	static void SetVisibleMat(DBConnectPtr pDBConnect, IntegerListPtr pVisibleMatList);
	/************************************************************************/

	/************************************************************************/
	// 关于“材料厚度”。

	// 保存“材料厚度”。
	static void SaveMatThick(DBConnectPtr pDBConnect, MaterialThicknessPtr pMatThick, BOOL bCreate, BOOL bSaveMatSize, BOOL bTransaction = TRUE);

	// 更新“材料厚度”的信息。
	static void UpdateMatThick(DBConnectPtr pDBConnect, LONGLONG iMatThickID, CString strName, double dThick, CString strComment, BOOL bTransaction = TRUE);

	// 删除“材料厚度”。
	static void DeleteMatName(DBConnectPtr pDBConnect, LONGLONG iMatName, BOOL bTransaction = TRUE);
	/************************************************************************/

	/************************************************************************/
	// 关于“材料尺寸”。

	// 保存“材料尺寸”。
	/* params:
	*  bCreate: whether create new MatSize or save an existing MatSize object.
	*/
	static void SaveMatSize(DBConnectPtr pDBConnect, IMaterialSizePtr pMaterialSize, BOOL bCreate, BOOL bTransaction = TRUE);

	// 删除“材料尺寸”。
	static void DeleteMatSize(DBConnectPtr pDBConnect, LONGLONG iMatSize, BOOL bTransaction = TRUE);

	// 保存默认的材料尺寸。
	static void SaveDflMatSize(DBConnectPtr pDBConnect, LONGLONG iMaterialSizeID);

	// 归还材料到材料库。
	static void ReturnMatSize(DBConnectPtr pDBConnect, IMaterialSizePtr pMaterialSize, int iRetNum, BOOL bTransaction = TRUE);
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
