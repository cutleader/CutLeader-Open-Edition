#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)

BEGIN_CUTLEADER_NAMESPACE()

// 编辑底图几何特征的命令基类。
class CanvasPatternEditCommandBase : public CommandBase
{
public:
	CanvasPatternEditCommandBase(const CString& strCommandName, const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData);
	~CanvasPatternEditCommandBase(void);

protected:
	// 克隆一份cad和cam数据，供undo使用。需要在每次调用Do函数时调用。
	void CloneCadAndCamData();

	// 恢复cad和cam数据，需要在每次调用UnDo函数时调用。
	void RestoreCadAndCamData();

protected:
	// 克隆一份cad和cam数据，供undo使用。
	PartCadDataPtr m_pPartCadData_backup;
	PartCamDataPtr m_pPartCamData_backup;

	// 实际存在于零件中的cad和cam数据。
	PartCadDataPtr m_pPartCadData;
	PartCamDataPtr m_pPartCamData;
};

END_CUTLEADER_NAMESPACE()
