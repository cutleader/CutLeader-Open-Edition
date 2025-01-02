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
	// ���С��и�򡱡�
	CutSequenceListPtr m_pCutSequences;

	// �������õ��Ĳ�����
	ConfigDataPtr m_pConfigData;
};

END_CUTLEADER_NAMESPACE()
