#pragma once

#include "clExpertLibCommon.h"

DECLARE_CUTLEADER_CLASS(ExpertLibNodeList)

BEGIN_CUTLEADER_NAMESPACE()

// the interface for the node in the expert library tree structure.
// notes:
//   1) here, "expert library tree structure" do not mean "tree" control, you know the structure of expert library is a tree structure.
enum EXPLIB_NODE_TYPE
{
	// the initial value.
	EXPLIB_NODE_NOT_INIT		= 0,

	// the root of the expert library tree.
	// notes:
	//   1) do not have ID.
	EXPLIB_NODE_ROOT			= 10,

	// “专家库项”节点。
	EXPLIB_NODE_EXPLIBITEM		= 11,

	// “参数配置”节点。
	EXPLIB_NODE_CONFIGURATION	= 20,


	/************************************************************************/
	// “配置项”节点。

	// the node of micro joint assignment rules.
	// notes:
	//   1) keep 参数配置ID.
	EXPLIB_NODE_CUT_MJ_ASSIGN	= 32,

	// the node of rules to assign the corner.
	// notes:
	//   1) keep 参数配置ID.
	EXPLIB_NODE_CORNER_ASSIGN	= 34,

	// the node of rules to assign lead in/out.
	// notes:
	//   1) keep 参数配置ID.
	EXPLIB_NODE_LEAD_ASSIGN		= 35,

	// the node of rules to assign pierce.
	// notes:
	//   1) keep 参数配置ID.
	EXPLIB_NODE_PIERCE_ASSIGN	= 36,

	// the node of rules to sequence.
	// notes:
	//   1) keep 参数配置ID.
	EXPLIB_NODE_GENERATE_SEQ	= 42,

	// the node of rules to generate NC code.
	// notes:
	//   1) keep 参数配置ID.
	EXPLIB_NODE_GENERATE_NC		= 43,

    // “刀具分配规则”节点。
    // notes:
    //   1) keep 参数配置ID.
    EXPLIB_NODE_TOOL_ASSIGN		= 44,

	// the nest config node.
	// notes:
	//   1) keep 参数配置ID.
	EXPLIB_NODE_NEST_PARAM		= 52,

	// 工艺参数
	// notes:
	//   1) keep 参数配置ID.
	EXPLIB_NODE_CutParam		= 53,
	/************************************************************************/
};

class ClExpertLib_Export IExpertLibNode
{
public:
	virtual void SetNodeName(CString strName) = 0;
	virtual CString GetNodeName() = 0;

	virtual void SetNodeType(EXPLIB_NODE_TYPE nodeType) = 0;
	virtual EXPLIB_NODE_TYPE GetNodeType() = 0;

	virtual void SetDataID(LONGLONG iDataID) = 0;
	virtual LONGLONG GetDataID() = 0;

	virtual ExpertLibNodeListPtr GetSubNodeList() = 0;
	virtual void SetSubNodeList(ExpertLibNodeListPtr pExpLibNodeList) = 0;
};

END_CUTLEADER_NAMESPACE()
