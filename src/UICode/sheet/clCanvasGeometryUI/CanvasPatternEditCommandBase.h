#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)

BEGIN_CUTLEADER_NAMESPACE()

// �༭��ͼ����������������ࡣ
class CanvasPatternEditCommandBase : public CommandBase
{
public:
	CanvasPatternEditCommandBase(const CString& strCommandName, const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData);
	~CanvasPatternEditCommandBase(void);

protected:
	// ��¡һ��cad��cam���ݣ���undoʹ�á���Ҫ��ÿ�ε���Do����ʱ���á�
	void CloneCadAndCamData();

	// �ָ�cad��cam���ݣ���Ҫ��ÿ�ε���UnDo����ʱ���á�
	void RestoreCadAndCamData();

protected:
	// ��¡һ��cad��cam���ݣ���undoʹ�á�
	PartCadDataPtr m_pPartCadData_backup;
	PartCamDataPtr m_pPartCamData_backup;

	// ʵ�ʴ���������е�cad��cam���ݡ�
	PartCadDataPtr m_pPartCadData;
	PartCamDataPtr m_pPartCamData;
};

END_CUTLEADER_NAMESPACE()
