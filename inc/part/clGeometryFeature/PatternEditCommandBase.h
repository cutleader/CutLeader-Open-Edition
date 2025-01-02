#pragma once

#include "clGeometryFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PartCadData)

BEGIN_CUTLEADER_NAMESPACE()

// 编辑几何特征的命令基类。
class ClGeometryFeature_Export PatternEditCommandBase : public CommandBase
{
public:
	PatternEditCommandBase(const CString& strCommandName, const EditorData& editorData, PartCadDataPtr pPartCadData);
	~PatternEditCommandBase(void);

protected:
	// 克隆一份cad数据，供undo使用。需要在每次调用Do函数时调用。
	void CloneCadData();

	// 恢复cad数据，需要在每次调用UnDo函数时调用。
	void RestoreCadData();

protected:
	// 克隆一份cad数据，供undo使用。
	PartCadDataPtr m_pPartCadData_backup;

	// 实际存在于零件中的cad和cam数据。
	PartCadDataPtr m_pPartCadData;
};

END_CUTLEADER_NAMESPACE()
