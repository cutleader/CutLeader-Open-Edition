#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

class ClPartLayout_Export PartPlacementExplodeCommand : public CommandBase
{
public:
	PartPlacementExplodeCommand(EditorData editorData, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartPlacementPtr pGridPmt, PartPlacementListPtr pNewPartPlacements);
	~PartPlacementExplodeCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PartPlacementListPtr m_pPartPmtList;
	PartInstanceListPtr m_pPartInstList;

	// 要炸散的零件阵列。
	PartPlacementPtr m_pGridPmt;

	// 炸散后的零件放置。
	PartPlacementListPtr m_pNewPartPmtList;

	// 缓存数据。
	// 注：
	//  1) 构造函数中初始化。
	PartInstanceListPtr m_pOldPartInsts; // 要炸散的零件阵列对应的零件实例。
	PartInstanceListPtr m_pNewPartInsts; // 炸散后的零件放置对应的零件实例。
};

END_CUTLEADER_NAMESPACE()
