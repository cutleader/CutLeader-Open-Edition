#pragma once

#include "clGeometryFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PartCadData)

BEGIN_CUTLEADER_NAMESPACE()

// �༭����������������ࡣ
class ClGeometryFeature_Export PatternEditCommandBase : public CommandBase
{
public:
	PatternEditCommandBase(const CString& strCommandName, const EditorData& editorData, PartCadDataPtr pPartCadData);
	~PatternEditCommandBase(void);

protected:
	// ��¡һ��cad���ݣ���undoʹ�á���Ҫ��ÿ�ε���Do����ʱ���á�
	void CloneCadData();

	// �ָ�cad���ݣ���Ҫ��ÿ�ε���UnDo����ʱ���á�
	void RestoreCadData();

protected:
	// ��¡һ��cad���ݣ���undoʹ�á�
	PartCadDataPtr m_pPartCadData_backup;

	// ʵ�ʴ���������е�cad��cam���ݡ�
	PartCadDataPtr m_pPartCadData;
};

END_CUTLEADER_NAMESPACE()
