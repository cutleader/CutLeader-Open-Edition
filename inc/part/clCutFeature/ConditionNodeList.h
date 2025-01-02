#pragma once

#include "clCutFeatureCommon.h"
#include "ClData.h"
#include "ConditionType.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ConditionNode)
DECLARE_CUTLEADER_CLASS(ConditionNodeList)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(CondValInfoList)

BEGIN_CUTLEADER_NAMESPACE()

// 条件节点集合。
class ClCutFeature_Export ConditionNodeList : public std::vector<ConditionNodePtr>, public ClData
{
public:
	ConditionNodeList(void);
	~ConditionNodeList(void);

public: // implement IData interface
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;
	IDataPtr Clone() const override;

public:
	/************************************************************************/	
	// about getting ConditionNode object.

	ConditionNodePtr GetItemByID(LONGLONG iCondNodeID);

	ConditionNodePtr GetByCondTypeID(LONGLONG iCondTypeID);

	// get ConditionNode list which fit "cateType".
	ConditionNodeListPtr GetByCategoryType(ConditionTypeListPtr pCondTypeList, COND_CATEGORY_TYPE cateType);
	/************************************************************************/

	void RemoveCondNode(ConditionNodePtr pCondNode);

public:
	// 创建“切割条件节点”。
	static ConditionNodeListPtr CreateCondNodeList(CondValInfoListPtr pCondValInfoList);
};

END_CUTLEADER_NAMESPACE()
