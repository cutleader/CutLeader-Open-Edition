#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(PartPlacementList)

BEGIN_CUTLEADER_NAMESPACE()

class ClPartLayout_Export PartPlacementTransformCommand : public CommandBase
{
public:
	/* params: 
	*  pPartPlacements: all part placements and instances in sheet.
	*  pOldPartPmts: the part pmts which will be transformed. this is the actual part pmt we want to update.
	*  pNewPartPlacements: this variable is used to assist the transform.
	*/
	PartPlacementTransformCommand(const CString& str, const EditorData& editorData, PartPlacementListPtr pPartPlacements,
		PartPlacementListPtr pOldPartPmts,PartPlacementListPtr pNewPartPlacements);
	~PartPlacementTransformCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	PartPlacementListPtr m_pPartPmtList;

	PartPlacementListPtr m_pOldPartPmtList;
	PartPlacementListPtr m_pNewPartPmtList;
};

END_CUTLEADER_NAMESPACE()
