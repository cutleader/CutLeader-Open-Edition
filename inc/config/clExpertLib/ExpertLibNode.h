#pragma once

#include "IExpertLibNode.h"

BEGIN_CUTLEADER_NAMESPACE()

///////////////////////////////////////////////////////////////////////////////

// the basic implementation for the node in the expert library tree structure.

// notes:
//   1) we use this structure to init the expert library tree, so we do not
//      need to load all expert library.
//   2) keep this object in the tree/listctrl, BUT "m_pSubNodeList" will be NULL.

///////////////////////////////////////////////////////////////////////////////

class ClExpertLib_Export ExpertLibNode : public IExpertLibNode
{
public:
	ExpertLibNode(void);
	ExpertLibNode(EXPLIB_NODE_TYPE nodeType);
	ExpertLibNode(CString strNodeName, EXPLIB_NODE_TYPE nodeType, LONGLONG iDataID);

	~ExpertLibNode(void);

public: // implement interface IExpertLibNode.
	virtual void SetNodeName(CString strName) { m_strNodeName = strName; }
	virtual CString GetNodeName() { return m_strNodeName; }

	virtual void SetNodeType(EXPLIB_NODE_TYPE nodeType) { m_nodeType = nodeType; }
	virtual EXPLIB_NODE_TYPE GetNodeType() { return m_nodeType; }

	virtual void SetDataID(LONGLONG iDataID) { m_iDataID = iDataID; }
	virtual LONGLONG GetDataID() { return m_iDataID; }

	virtual ExpertLibNodeListPtr GetSubNodeList() { return m_pSubNodeList; }
	virtual void SetSubNodeList(ExpertLibNodeListPtr pSubNodeList) { m_pSubNodeList = pSubNodeList; }

private:
	void Construct();

private:
	// the name of the node.
	CString m_strNodeName;

	// the node type.
	EXPLIB_NODE_TYPE m_nodeType;

	// the id of the data.
	// notes:
	//   1) we can load the data use this variable. BUT sometimes the ID is
	//      not the ID of the node data. e.g., the ID of expert library item
	//      is the ID of the machine.
	LONGLONG m_iDataID;

	// the sub nodes of this node.
	// notes:
	//   1) if no sub node under this node, this variable will be NULL.
	ExpertLibNodeListPtr m_pSubNodeList;
};

END_CUTLEADER_NAMESPACE()
