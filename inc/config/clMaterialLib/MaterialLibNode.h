#pragma once

#include "clMaterialLibCommon.h"
#include "IMaterialLibNode.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClMaterialLib_Export MaterialLibNode : public IMaterialLibNode
{
public:
	MaterialLibNode(void);
	MaterialLibNode(MATLIB_NODE_TYPE nodeType);
	MaterialLibNode(CString strNodeName, MATLIB_NODE_TYPE nodeType, LONGLONG iDataID);
	virtual ~MaterialLibNode(void);

public: // implement interface IMaterialLibNode.
	virtual void SetNodeName(CString strName) override { m_strNodeName = strName; }
	virtual CString GetNodeName() const override { return m_strNodeName; }

	virtual void SetMatLibNodeType(MATLIB_NODE_TYPE nodeType) override { m_nodeType = nodeType; }
	virtual MATLIB_NODE_TYPE GetMatLibNodeType() const override { return m_nodeType; }

	virtual void SetDataID(LONGLONG iDataID) override { m_iDataID = iDataID; }
	virtual LONGLONG GetDataID() const override { return m_iDataID; }

	virtual MaterialLibNodeListPtr GetSubNodeList() const override { return m_pSubNodeList; }
	virtual void SetSubNodeList(MaterialLibNodeListPtr pSubNodeList) override { m_pSubNodeList = pSubNodeList; }

private:
	void Construct();

private:
	// the name of the node.
	CString m_strNodeName;

	// the node type.
	MATLIB_NODE_TYPE m_nodeType;

	// the id of the data.
	LONGLONG m_iDataID;

	// the sub nodes of this node.
	// notes:
	//   1) if no sub node under this node, this variable will be NULL.
	MaterialLibNodeListPtr m_pSubNodeList;
};

END_CUTLEADER_NAMESPACE()
