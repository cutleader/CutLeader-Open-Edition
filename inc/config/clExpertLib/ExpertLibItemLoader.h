#pragma once

#include "clExpertLibCommon.h"
#include "DataProperty.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(ExpertLibItem)
DECLARE_CUTLEADER_CLASS(MachineItem)

BEGIN_CUTLEADER_NAMESPACE()

class IExpertLibNode;
typedef boost::shared_ptr<IExpertLibNode> ExpLibNodePtr;

// the ExpertLibItem load type.
typedef enum tagExpLibItemLoadType
{
	// only "MacDesc" data and "BusinessData" obj will be loaded.
	EXPLIBITEM_LOAD_BASIC			= 0,

	// load all data.
	EXPLIBITEM_LOAD_ALL				= 1,
} EXPLIBITEM_LOAD_TYPE;

class ClExpertLib_Export ExpertLibItemLoader
{
public:
	/************************************************************************/
	// about getting basic info of ExpertLibItem.

	// �õ����С�ר�ҿ������Ϣ��
	/* params:
	*  bDropReserved: whether we load the ExpertLibItem for the reserved machine.
	*/
	// notes:
	//   1) data in DataProperty:
	//      a) expLib item id.
	//      b) expLib Item name.
	//      c) expLib Item comment.
	//      d) ����ID��
	static std::vector<DataProperty> GetExpLibItemInfoList(DBConnectPtr pDBConnect, BOOL bDropReserved = TRUE);
	static std::vector<DataProperty> GetExpLibItemInfoList(DBConnectPtr pDBConnect, DBConnectPtr pMacLibCon, BOOL bDropReserved = TRUE);

	// get ExpertLibItem name by id.
	static CString GetExpLibItemName(DBConnectPtr pDBConnect, LONGLONG iExpLibItemID);

	// get ExpertLibItem ID by name.
	// notes:
	//   1) the ExpertLibItem name is unique, so we can call this.
	static LONGLONG GetExpLibItemIDByName(DBConnectPtr pDBConnect, CString strExpLibItem);
	/************************************************************************/

	/************************************************************************/
	// about the relations with machine.

	// get the ExpertLibItem ID according to the machine.
	static LONGLONG GetExpLibItemID(DBConnectPtr pDBConnect, LONGLONG iMacID);

	// get machine ID by expert library item id.
	static LONGLONG GetMacIDByExpLibItem(DBConnectPtr pDBConnect, LONGLONG iExpLibItem);
	/************************************************************************/

	/************************************************************************/
	// about load expLib item.

	// ���ء�ר�ҿ����
	// notes:
	//   1) caller should prepare the mac item object.
	static ExpertLibItemPtr LoadExpLibItemEx(DBConnectPtr pDBConnect, LONGLONG iExpLibItemID, EXPLIBITEM_LOAD_TYPE loadType, MachineItemPtr pMacItem);

	// ���ء�ר�ҿ����
	static ExpertLibItemPtr LoadExpLibItem(DBConnectPtr pDBConnect, LONGLONG iExpLibItemID, EXPLIBITEM_LOAD_TYPE loadType);

	// ����ר�ҿ����ڵ㡣
	// ע:
	//   1) the items returned are in a proper order.
	//   2) ֻ���ؿɼ��Ļ�����Ӧ��ר�ҿ⡣
	static ExpLibNodePtr LoadExpLibTree(DBConnectPtr pDBConnect, BOOL bDropReserved = TRUE, BOOL bLoadVisible = TRUE);

	// ��ʼ����ר�ҿ���ڵ���ӽڵ㡣
	// notes:
	//   1) expert library item ID MUST be set in "pExpLibItemNode".
	static void InitExpLibItem(DBConnectPtr pDBConnect, ExpLibNodePtr pExpLibItemNode);
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
