#pragma once

#include "clMachineLibCommon.h"
#include "IMachineLibNode.h"

DECLARE_CUTLEADER_CLASS(MachineLibNodeList)

BEGIN_CUTLEADER_NAMESPACE()

// the basic implementation for the node in the macLib tree structure.
// notes:
//   1) we use this structure to init the macLib tree, so do not need to load all macLib.
class ClMachineLib_Export MachineLibNode : public IMachineLibNode
{
public:
	MachineLibNode(MACLIB_NODE_TYPE nodeType);
	MachineLibNode(CString strNodeName, MACLIB_NODE_TYPE nodeType, LONGLONG iDataID);

	~MachineLibNode(void);

public: // implement interface IMachineLibNode.
	virtual void SetNodeName(CString strName) { m_strNodeName = strName; }
	virtual CString GetNodeName() { return m_strNodeName; }

	virtual void SetMacLibNodeType(MACLIB_NODE_TYPE nodeType) { m_nodeType = nodeType; }
	virtual MACLIB_NODE_TYPE GetMacLibNodeType() { return m_nodeType; }

	virtual void SetDataID(LONGLONG iDataID) { m_iDataID = iDataID; }
	virtual LONGLONG GetDataID() { return m_iDataID; }

	virtual MachineLibNodeListPtr GetSubNodeList() { return m_pSubNodeList; }
	virtual void SetSubNodeList(MachineLibNodeListPtr pSubNodeList) { m_pSubNodeList = pSubNodeList; }

private:
	void Construct();

private:
	// the name of the node.
	CString m_strNodeName;

	// the node type.
	MACLIB_NODE_TYPE m_nodeType;

	// the id of the data.
	// notes:
	//   1) we can load the data use this variable. BUT sometimes the ID is
	//      not the ID of the node data. e.g., the ID of expert library item
	//      is the ID of the machine.
	LONGLONG m_iDataID;

	// the sub nodes of this node.
	// notes:
	//   1) if no sub node under this node, this variable will be NULL.
	MachineLibNodeListPtr m_pSubNodeList;
};

END_CUTLEADER_NAMESPACE()
