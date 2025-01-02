#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(PartInstance)

BEGIN_CUTLEADER_NAMESPACE()

// 该命令创建一个零件阵列。
// 注:
// 1) 把原有的零件放置更改为阵列，而不是新建一个零件放置，这么做的目的是为了保持之前的加工工艺。
class ClPartLayout_Export PartPlacementGridCommand : public CommandBase
{
public:
	PartPlacementGridCommand(const EditorData& editorData, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
		PartPlacementPtr pPartPlacement, int iRowCount, int iColCount, double dSpacingX, double dSpacingY);
	~PartPlacementGridCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	PartPlacementListPtr m_pPartPlacements;
	PartInstanceListPtr m_pPartInstances;
	PartPlacementPtr m_pPartPlacement;

	// 要生成这样的阵列。
	int	m_iRowCount;
	int	m_iColCount;
	double m_dSpacingX;
	double m_dSpacingY;

	// 备份零件实例，undo时该零件实例要恢复回去，这样其他一些数据也能恢复回去，比如工序。
	PartInstancePtr m_pOldPartInstance;
};

END_CUTLEADER_NAMESPACE()
