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
	std::map<LONGLONG, LineArc2DListPtr> uselessRegion; // ��һ�������Ǳ��е�loopIndex�ֶΡ�
};

class ClMaterialLib_Export MaterialLibLoader
{
public:
	/************************************************************************/
	// ���ڡ����ϡ���

	// �������в��ϡ�
	static MaterialListPtr LoadAllMat(DBConnectPtr pDBConnect, BOOL bLoadVisible, BOOL bNotLoadMaterialSize4Canvas);

	// ���ز��Ͽ����нڵ㡣
	// ע��
	//  1) ֻ���ؿɼ��Ĳ��ϡ�
	static IMaterialLibNodePtr LoadMatLibTree(DBConnectPtr pDBConnect, BOOL bLoadVisible, BOOL bNotLoadMaterialSize4Canvas);

	// ����ָ���Ĳ��Ͻڵ㡣
	static IMaterialLibNodePtr LoadMatTreeNode(DBConnectPtr pDBConnect, LONGLONG iMatID, BOOL bNotLoadMaterialSize4Canvas);

	// �õ����ϵ����ԡ�
	// ע��
	//   1) �����еĸ����
	//      a) ����ID��
	//      b) �������ơ�
	//      c) ����
	//      d) ��ע
	//      e) ��������
	//      f) �޸�����
	static BOOL GetMatProp(DBConnectPtr pDBConnect, LONGLONG iMatID, DataProperty& prop);

	// �ӡ����Ϻ�ȡ��õ�����ID��
	static LONGLONG GetMatID(DBConnectPtr pDBConnect, LONGLONG iMatThickID);
	/************************************************************************/


	/************************************************************************/
	// ���ڡ����Ϻ�ȡ���

	// �õ����Ϻ�ȵ����ԡ�
	// ע��
	//   1) �����еĸ����
	//      1) ���Ϻ��ID��
	//      2) ����ID��
	//      3) ����
	//      4) ���
	//      5) ��ע
	static BOOL GetMatThickProp(DBConnectPtr pDBConnect, LONGLONG iMatThickID, DataProperty& prop);

	// �õ������Ϻ�ȡ������ԡ�
	// ע��
	//   1) �����еĸ����
	//      1) ���Ϻ��ID��
	//      2) ����ID
	//      3) ����
	//      4) ���
	//      5) ��ע
	static std::vector<DataProperty> GetMatThickProp(DBConnectPtr pDBConnect, LONGLONG iMatID);

	// �ӡ����ϳߴ硱�õ������Ϻ�ȡ���
	static LONGLONG GetMatThickID(DBConnectPtr pDBConnect, LONGLONG iMaterialSizeID);
	/************************************************************************/


	/************************************************************************/
	// ���ڡ����ϳߴ硱��

	// �õ������ϳߴ硱�����ԡ�
	// ע:
	//   1) �����еĸ���� 
	//      1) ��������
	//      2) ���ֵ
	//      3) ���
	//      4) �߶�
	//      5) ����ID��
	//      6) ���Ϻ��ID��
	//      7) ���ϳߴ����ơ�
	//      8) �ò��Ͽ���������
	static BOOL GetMatSizeProp(DBConnectPtr pDBConnect, LONGLONG iMaterialSizeID, DataProperty& prop);

	// �õ���������Ϻ�ȡ������С����ϳߴ硱�����ԡ�
	// ע��
	//   1) �����еĸ����
	//      1) ���ϳߴ�ID��
	//      2) ��������
	//      3) �Ƿ�����
	//      4) ���
	//      5) �߶�
	//      6) ����
	//      7) ������
	//      8) ��ע
	//      9) ���ϳߴ����ơ�
	static std::vector<DataProperty> GetMatSizeProp(DBConnectPtr pDBConnect, LONGLONG iMatThickID, BOOL bNotLoadMaterialSize4Canvas);

	// ����ָ���Ĳ��ϳߴ硣
	static IMaterialSizePtr LoadMatSize(DBConnectPtr pDBConnect, LONGLONG iMaterialSizeID);

	// �õ�Ĭ�ϵĲ��ϳߴ硣
	static LONGLONG GetDflMatSize(DBConnectPtr pDBConnect);

	// �õ�ĳ�����Ϻ������ĵ�ͼ���ϡ�
	static LONGLONG GetCanvasMaterialSizeID(DBConnectPtr pDBConnect, LONGLONG iMaterialThicknessID);
	/************************************************************************/

private:
	/************************************************************************/
	// ���ڡ����Ϻ�ȡ���

	// �������С����Ϻ�ȡ����ݡ�
	// ע��
	//  1) ����map�ĵ�һ�������ǲ���ID��
	static std::map<LONGLONG, std::vector<MatThickData>> LoadMatThickData(DBConnectPtr pDBConnect);

	// �������С����Ϻ�ȡ��ڵ㡣
	// ע��
	//  1) ����map�ĵ�һ�������ǲ���ID��
	static std::map<LONGLONG, MaterialLibNodeListPtr> LoadMatThickNodes(DBConnectPtr pDBConnect);
	/************************************************************************/


	/************************************************************************/
	// ���ڡ����ϳߴ硱��

	// �������С����ϳߴ硱���ݡ�
	// ע��
	//  1) ����map�ĵ�һ�������ǲ��Ϻ��ID��
	static std::map<LONGLONG, std::vector<MatSizeData>> LoadMatSizeData(DBConnectPtr pDBConnect, BOOL bNotLoadMaterialSize4Canvas);

	// �������С���������ϡ����������ݡ�
	// ע��
	//  1) ����map�ĵ�һ�������ǲ��ϳߴ�ID��
	static std::map<LONGLONG, PolyMatData> LoadPolyMatData(DBConnectPtr pDBConnect);

	// ����ָ������������ϡ����������ݡ�
	static PolyMatData LoadPolyMatData(DBConnectPtr pDBConnect, LONGLONG iPolyMatID);

	// �������С����Ϻ�ȡ��ڵ㡣
	// ע��
	//  1) ����map�ĵ�һ�������ǲ��Ϻ��ID��
	static std::map<LONGLONG, MaterialLibNodeListPtr> LoadMatSizeNodes(DBConnectPtr pDBConnect, BOOL bNotLoadMaterialSize4Canvas);
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
