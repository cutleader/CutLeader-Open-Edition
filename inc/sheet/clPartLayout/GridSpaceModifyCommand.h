#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(PartPlacement)

BEGIN_CUTLEADER_NAMESPACE()

// 这个命令修改阵列的行/列间距。
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

	// 老的行列间距。
	double m_dOldXSpace;
	double m_dOldYSpace;
	
	// 新的行列间距。
	double m_dNewXSpace;
	double m_dNewYSpace;
};

END_CUTLEADER_NAMESPACE()
