#pragma once

#include "clGeometryFeatureCommon.h"
#include "PatternEditCommandBase.h"
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PartCadData)

BEGIN_CUTLEADER_NAMESPACE()

// the command that copy pattern(s).
class ClGeometryFeature_Export PatternCopyCommand : public PatternEditCommandBase
{
public:
	PatternCopyCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PatternListPtr pPatterns_willCopy,
		const std::vector<LONGLONG>& textStructs_willCopy, const Matrix2D& transformMatrix);
	~PatternCopyCommand(void);

public: // implements ICommand interface.
	virtual void Do();
	virtual void UnDo();

private:
	// ϣ�������ļ������������ֶ����Լ��������ĸ�λ�á�
	PatternListPtr m_pPatterns_willCopy;
	const std::vector<LONGLONG> m_textStructs_willCopy;
	const Matrix2D m_transformMatrix;
};

END_CUTLEADER_NAMESPACE()
