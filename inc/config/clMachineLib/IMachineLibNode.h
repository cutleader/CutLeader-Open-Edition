#pragma once

#include "clMachineLibCommon.h"

DECLARE_CUTLEADER_CLASS(MachineLibNodeList)

BEGIN_CUTLEADER_NAMESPACE()

typedef enum tagMacLibNodeType
{
	// the initial value.
	MACLIB_NODE_NOT_INIT		= 0,

	// the root of the macLib tree.
	// notes:
	//   1) do not have ID.
	MACLIB_NODE_ROOT			= 10,

	// the machine item node.
	// notes:
	//   1) keep mac item ID.
	MACLIB_NODE_MACLIB_ITEM		= 11,

	// the machine param node.
	// notes:
	//   1) keep machine item ID.
	MACLIB_NODE_MAC_PARAM		= 21,

	// the machine condition node.
	// notes:
	//   1) keep machine item ID.
	MACLIB_NODE_MAC_COND		= 22,

	// the NC config types node.
	// notes:
	//   1) keep machine item ID.
	MACLIB_NODE_MAC_NCCONFIG	= 23,

	// 工艺库节点。
	// notes:
	//   1) keep machine item ID.
	MACLIB_NODE_CutParamLib		= 24,
} MACLIB_NODE_TYPE;

// the interface for the node in the mac library tree structure.
class ClMachineLib_Export IMachineLibNode
{
public:
	virtual void SetNodeName(CString strName) = 0;
	virtual CString GetNodeName() = 0;

	virtual void SetMacLibNodeType(MACLIB_NODE_TYPE nodeType) = 0;
	virtual MACLIB_NODE_TYPE GetMacLibNodeType() = 0;

	virtual void SetDataID(LONGLONG iDataID) = 0;
	virtual LONGLONG GetDataID() = 0;

	virtual MachineLibNodeListPtr GetSubNodeList() = 0;
	virtual void SetSubNodeList(MachineLibNodeListPtr pMacLibNodeList) = 0;
};

END_CUTLEADER_NAMESPACE()
