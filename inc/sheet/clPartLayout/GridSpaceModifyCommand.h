#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(PartPlacement)

BEGIN_CUTLEADER_NAMESPACE()

// ��������޸����е���/�м�ࡣ
class ClPartLayout_Export GridSpaceModifyCommand : public CommandBase
{
public:
	GridSpaceModifyCommand(EditorData editorData, PartPlacementPtr pPartPlacement, double dOldXSpace, double dOldYSpace, double dNewXSpace, double dNewYSpace);
	virtual ~GridSpaceModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PartPlacementPtr m_pPartPlacement;

	// �ϵ����м�ࡣ
	double m_dOldXSpace;
	double m_dOldYSpace;
	
	// �µ����м�ࡣ
	double m_dNewXSpace;
	double m_dNewYSpace;
};

END_CUTLEADER_NAMESPACE()
