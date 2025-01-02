#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(PartPlacementList)

BEGIN_CUTLEADER_NAMESPACE()

// 这个命令用矩阵变换。
class ClPartLayout_Export PartPlacementTransformCommand_2 : public CommandBase
{
public:
	PartPlacementTransformCommand_2(const CString& str, const EditorData& editorData, PartPlacementListPtr pAllPartPlacements,
		PartPlacementListPtr pPartPlacements,const Matrix2D& transformMatrix);
	~PartPlacementTransformCommand_2(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	PartPlacementListPtr m_pAllPartPlacements;

	PartPlacementListPtr m_pPartPlacements;
	const Matrix2D m_transformMatrix;
};

END_CUTLEADER_NAMESPACE()
