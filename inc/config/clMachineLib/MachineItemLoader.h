#pragma once

#include "clMachineLibCommon.h"
#include "DataProperty.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(MachineItem)
DECLARE_CUTLEADER_CLASS(MachineLibNode)
DECLARE_CUTLEADER_CLASS(MachineParam)
DECLARE_CUTLEADER_CLASS(DriverData)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(ConditionType)
DECLARE_CUTLEADER_CLASS(NCConfigTypeList)
DECLARE_CUTLEADER_CLASS(NCConfigType)

BEGIN_CUTLEADER_NAMESPACE()

// this class load machine desc from the database.
class ClMachineLib_Export MachineItemLoader
{
public:
	/************************************************************************/
	// about load the mac item.

	// load mac item by ID.
	static MachineItemPtr LoadMacItem(DBConnectPtr pDBConnect, LONGLONG iMacItemID);

	// get the ID of the reserved mac item.
	static LONGLONG GetReserverdMacItem(DBConnectPtr pDBConnect);

	// 得到可见的机器。
	static IntegerListPtr GetVisibleMacList(DBConnectPtr pDBConnect);

	// get prop of mac item.
	// notes:
	//   1) data in a DataProperty:
	//      1) id
	//      2) name
	//      3) comment
	//      4) create date
	//      5) modify date
	static BOOL GetMacItemProp(DBConnectPtr pDBConnect, LONGLONG iMacItemID, DataProperty& dataProp);

	// get prop list of mac item.
	// notes:
	//   1) data in a DataProperty:
	//      1) id
	//      2) name
	//      3) comment
	//      4) create date
	//      5) modify date
	//      6) 是否可见。
	static BOOL GetMacItemPropList(DBConnectPtr pDBConnect, std::vector<DataProperty>& propList, BOOL bDropReserved = TRUE, BOOL bLoadVisible = TRUE);
	/************************************************************************/


	// load all macLib tree nodes.
	// notes:
	//   1) the items returned are in a proper order.
	//   2) 只显示可见的机器。
	static MachineLibNodePtr LoadMacLibTree(DBConnectPtr pDBConnect, BOOL bDropReserved = TRUE, BOOL bLoadVisible = TRUE);

	// load mac param.
	static MachineParamPtr LoadMacParam(DBConnectPtr pDBConnect, LONGLONG iMacItemID);


	/************************************************************************/
	// about load driver.

	// load the machine driver.
	static DriverDataPtr LoadDriverData(DBConnectPtr pDBConnect, LONGLONG iMacItemID);

	// get the driver version.
	static CString GetDriverVersion(DriverDataPtr pDriverData);
	/************************************************************************/


	/************************************************************************/
	// about load mac condition.

	// load all conditions of the machine.
	static ConditionTypeListPtr LoadMacConds(DBConnectPtr pDBConnect, LONGLONG iMacItemID);

	// load the condition by ID.
	static ConditionTypePtr LoadMacCond(DBConnectPtr pDBConnect, LONGLONG iCondTypeId);
	/************************************************************************/


	/************************************************************************/
	// about load mac condition.

	// load all nc config of the machine.
	static NCConfigTypeListPtr LoadMacNCConfigType(DBConnectPtr pDBConnect, LONGLONG iMacItemID);

	// load the nc config by ID.
	static NCConfigTypePtr LoadNCConfigTypeByID(DBConnectPtr pDBConnect, LONGLONG iID);
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
