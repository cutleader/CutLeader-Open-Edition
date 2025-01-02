#pragma once

#include "clCutSequenceCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(ConfigData)

BEGIN_CUTLEADER_NAMESPACE()

class SequenceCommandBase : public CommandBase
{
public:
	SequenceCommandBase(const CString& strCommandName, const EditorData& editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData);
	virtual ~SequenceCommandBase(void);

protected:
	// 所有“切割工序”。
	CutSequenceListPtr m_pCutSequences;

	// 工序处理用到的参数。
	ConfigDataPtr m_pConfigData;
};

END_CUTLEADER_NAMESPACE()
