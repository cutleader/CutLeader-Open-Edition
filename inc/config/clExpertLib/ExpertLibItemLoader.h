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

	// 得到所有“专家库项”的信息。
	/* params:
	*  bDropReserved: whether we load the ExpertLibItem for the reserved machine.
	*/
	// notes:
	//   1) data in DataProperty:
	//      a) expLib item id.
	//      b) expLib Item name.
	//      c) expLib Item comment.
	//      d) 机器ID。
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

	// 加载“专家库项”。
	// notes:
	//   1) caller should prepare the mac item object.
	static ExpertLibItemPtr LoadExpLibItemEx(DBConnectPtr pDBConnect, LONGLONG iExpLibItemID, EXPLIBITEM_LOAD_TYPE loadType, MachineItemPtr pMacItem);

	// 加载“专家库项”。
	static ExpertLibItemPtr LoadExpLibItem(DBConnectPtr pDBConnect, LONGLONG iExpLibItemID, EXPLIBITEM_LOAD_TYPE loadType);

	// 加载专家库树节点。
	// 注:
	//   1) the items returned are in a proper order.
	//   2) 只加载可见的机器对应的专家库。
	static ExpLibNodePtr LoadExpLibTree(DBConnectPtr pDBConnect, BOOL bDropReserved = TRUE, BOOL bLoadVisible = TRUE);

	// 初始化“专家库项”节点的子节点。
	// notes:
	//   1) expert library item ID MUST be set in "pExpLibItemNode".
	static void InitExpLibItem(DBConnectPtr pDBConnect, ExpLibNodePtr pExpLibItemNode);
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
