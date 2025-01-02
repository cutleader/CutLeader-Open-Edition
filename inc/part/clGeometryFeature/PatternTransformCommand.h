#pragma once

#include "clGeometryFeatureCommon.h"
#include "PatternEditCommandBase.h"
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(TextStructList)
DECLARE_CUTLEADER_CLASS(PatternList)

BEGIN_CUTLEADER_NAMESPACE()

// this command will transform the patterns.
class ClGeometryFeature_Export PatternTransformCommand : public PatternEditCommandBase
{
public:
	PatternTransformCommand(const EditorData& editorData, const CString& strName, PartCadDataPtr pPartCadData,
		const std::vector<LONGLONG>& patterns_willTransform, const std::vector<Matrix2D>& patternTransformMatrixData,
		const std::vector<LONGLONG>& textStructs_willTransform, const std::vector<Matrix2D>& textTransformMatrixData);
	~PatternTransformCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// ϣ���任�ļ����������Լ��任���ĸ�λ�á�
	std::vector<LONGLONG> m_patterns_willTransform;
	const std::vector<Matrix2D> m_patternTransformMatrixData;

	// ϣ���任�����֣��Լ��任���ĸ�λ�á�
	std::vector<LONGLONG> m_textStructs_willTransform;
	const std::vector<Matrix2D> m_textTransformMatrixData;
};

END_CUTLEADER_NAMESPACE()
