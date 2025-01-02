#pragma once

#include "clMaterialLibCommon.h"

DECLARE_CUTLEADER_CLASS(MaterialLibNodeList)

BEGIN_CUTLEADER_NAMESPACE()

typedef enum tagMatLibNodeType
{
	// the initial value.
	MATLIB_NODE_NOT_INIT	= 0,

	// the root of the material library tree.
	// notes:
	//   1) do not have ID.
	MATLIB_NODE_ROOT		= 10,

	// the material node.
	// notes:
	//   1) keep material ID.
	MATLIB_NODE_MAT			= 20,

	// the material thickness node.
	// notes:
	//   1) keep material thickness ID.
	MATLIB_NODE_MATTHICK	= 21,

	// the material size node.
	// notes:
	//   1) keep material size ID.
	MATLIB_NODE_MATSIZE		= 22,
} MATLIB_NODE_TYPE;

class ClMaterialLib_Export IMaterialLibNode
{
public:
	virtual void SetNodeName(CString strName) = 0;
	virtual CString GetNodeName() const = 0;

	virtual void SetMatLibNodeType(MATLIB_NODE_TYPE nodeType) = 0;
	virtual MATLIB_NODE_TYPE GetMatLibNodeType() const = 0;

	virtual void SetDataID(LONGLONG iDataID) = 0;
	virtual LONGLONG GetDataID() const = 0;

	virtual MaterialLibNodeListPtr GetSubNodeList() const = 0;
	virtual void SetSubNodeList(MaterialLibNodeListPtr pMatLibNodeList) = 0;
};

END_CUTLEADER_NAMESPACE()
