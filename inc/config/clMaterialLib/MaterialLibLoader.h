#pragma once

#include "IMaterialSize.h"
#include <map>
#include <vector>

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(MaterialList)
DECLARE_CUTLEADER_CLASS(IMaterialLibNode)
DECLARE_CUTLEADER_CLASS(MaterialLibNodeList)

BEGIN_CUTLEADER_NAMESPACE()

class MatThickData
{
public:
	LONGLONG iID;
	CString strName;
	double dThickness;
	CString strComment;
};

class MatSizeData
{
public:
	LONGLONG iID;
	CString strName;
	MAT_SIZE_TYPE iType;
	BOOL bRemnant;
	double dWidth;
	double dHeight;
	int iTotalNum;
	int iUsedNum;
	CString strComment;
};

class PolyMatData
{
public:
	LineArc2DListPtr pOuterLoop;
	std::map<LONGLONG, LineArc2DListPtr> uselessRegion; // 第一个参数是表中的loopIndex字段。
};

class ClMaterialLib_Export MaterialLibLoader
{
public:
	/************************************************************************/
	// 关于“材料”。

	// 加载所有材料。
	static MaterialListPtr LoadAllMat(DBConnectPtr pDBConnect, BOOL bLoadVisible, BOOL bNotLoadMaterialSize4Canvas);

	// 加载材料库所有节点。
	// 注：
	//  1) 只加载可见的材料。
	static IMaterialLibNodePtr LoadMatLibTree(DBConnectPtr pDBConnect, BOOL bLoadVisible, BOOL bNotLoadMaterialSize4Canvas);

	// 加载指定的材料节点。
	static IMaterialLibNodePtr LoadMatTreeNode(DBConnectPtr pDBConnect, LONGLONG iMatID, BOOL bNotLoadMaterialSize4Canvas);

	// 得到材料的属性。
	// 注：
	//   1) 属性中的各个项。
	//      a) 材料ID。
	//      b) 材料名称。
	//      c) 比重
	//      d) 备注
	//      e) 创建日期
	//      f) 修改日期
	static BOOL GetMatProp(DBConnectPtr pDBConnect, LONGLONG iMatID, DataProperty& prop);

	// 从“材料厚度”得到材料ID。
	static LONGLONG GetMatID(DBConnectPtr pDBConnect, LONGLONG iMatThickID);
	/************************************************************************/


	/************************************************************************/
	// 关于“材料厚度”。

	// 得到材料厚度的属性。
	// 注：
	//   1) 属性中的各个项。
	//      1) 材料厚度ID。
	//      2) 材料ID。
	//      3) 名称
	//      4) 厚度
	//      5) 备注
	static BOOL GetMatThickProp(DBConnectPtr pDBConnect, LONGLONG iMatThickID, DataProperty& prop);

	// 得到“材料厚度”的属性。
	// 注：
	//   1) 属性中的各个项。
	//      1) 材料厚度ID。
	//      2) 材料ID
	//      3) 名称
	//      4) 厚度
	//      5) 备注
	static std::vector<DataProperty> GetMatThickProp(DBConnectPtr pDBConnect, LONGLONG iMatID);

	// 从“材料尺寸”得到“材料厚度”。
	static LONGLONG GetMatThickID(DBConnectPtr pDBConnect, LONGLONG iMaterialSizeID);
	/************************************************************************/


	/************************************************************************/
	// 关于“材料尺寸”。

	// 得到“材料尺寸”的属性。
	// 注:
	//   1) 属性中的各个项： 
	//      1) 材料名称
	//      2) 厚度值
	//      3) 宽度
	//      4) 高度
	//      5) 材料ID。
	//      6) 材料厚度ID。
	//      7) 材料尺寸名称。
	//      8) 该材料可用张数。
	static BOOL GetMatSizeProp(DBConnectPtr pDBConnect, LONGLONG iMaterialSizeID, DataProperty& prop);

	// 得到这个“材料厚度”下所有“材料尺寸”的属性。
	// 注：
	//   1) 属性中的各个项：
	//      1) 材料尺寸ID。
	//      2) 材料类型
	//      3) 是否余料
	//      4) 宽度
	//      5) 高度
	//      6) 总数
	//      7) 可用数
	//      8) 备注
	//      9) 材料尺寸名称。
	static std::vector<DataProperty> GetMatSizeProp(DBConnectPtr pDBConnect, LONGLONG iMatThickID, BOOL bNotLoadMaterialSize4Canvas);

	// 加载指定的材料尺寸。
	static IMaterialSizePtr LoadMatSize(DBConnectPtr pDBConnect, LONGLONG iMaterialSizeID);

	// 得到默认的材料尺寸。
	static LONGLONG GetDflMatSize(DBConnectPtr pDBConnect);

	// 得到某个材料厚度下面的底图材料。
	static LONGLONG GetCanvasMaterialSizeID(DBConnectPtr pDBConnect, LONGLONG iMaterialThicknessID);
	/************************************************************************/

private:
	/************************************************************************/
	// 关于“材料厚度”。

	// 加载所有“材料厚度”数据。
	// 注：
	//  1) 返回map的第一个参数是材料ID。
	static std::map<LONGLONG, std::vector<MatThickData>> LoadMatThickData(DBConnectPtr pDBConnect);

	// 加载所有“材料厚度”节点。
	// 注：
	//  1) 返回map的第一个参数是材料ID。
	static std::map<LONGLONG, MaterialLibNodeListPtr> LoadMatThickNodes(DBConnectPtr pDBConnect);
	/************************************************************************/


	/************************************************************************/
	// 关于“材料尺寸”。

	// 加载所有“材料尺寸”数据。
	// 注：
	//  1) 返回map的第一个参数是材料厚度ID。
	static std::map<LONGLONG, std::vector<MatSizeData>> LoadMatSizeData(DBConnectPtr pDBConnect, BOOL bNotLoadMaterialSize4Canvas);

	// 加载所有“不规则材料”的轮廓数据。
	// 注：
	//  1) 返回map的第一个参数是材料尺寸ID。
	static std::map<LONGLONG, PolyMatData> LoadPolyMatData(DBConnectPtr pDBConnect);

	// 加载指定“不规则材料”的轮廓数据。
	static PolyMatData LoadPolyMatData(DBConnectPtr pDBConnect, LONGLONG iPolyMatID);

	// 加载所有“材料厚度”节点。
	// 注：
	//  1) 返回map的第一个参数是材料厚度ID。
	static std::map<LONGLONG, MaterialLibNodeListPtr> LoadMatSizeNodes(DBConnectPtr pDBConnect, BOOL bNotLoadMaterialSize4Canvas);
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
