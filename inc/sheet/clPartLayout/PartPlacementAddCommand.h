#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// the command that add a part placement into sheet.
// notes: 
//   1) when add the part placement, we should also add the relative part instance.
class ClPartLayout_Export PartPlacementAddCommand : public CommandBase
{
public:
	PartPlacementAddCommand(const EditorData& editorData, PartPlacementListPtr pPartPlacements, PartPlacementPtr pPartPlacement, PartInstanceListPtr pPartInstances);
	~PartPlacementAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PartPlacementListPtr m_pPartPmtList;
	PartPlacementPtr m_pPartPlacement;

	PartInstanceListPtr m_pPartInstList;
};

END_CUTLEADER_NAMESPACE()
