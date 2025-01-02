#pragma once

#include "clCanvasGeometryUICommon.h"
#include "CanvasPatternEditCommandBase.h"
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)
DECLARE_CUTLEADER_CLASS(TextStructList)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(CanvasLoopCopyCommand)

BEGIN_CUTLEADER_NAMESPACE()

// �����������������ͼ�ϵ������������ϵļӹ�����ҲҪһ�𿽱���
class CanvasLoopCopyCommand : public CanvasPatternEditCommandBase
{
public:
	CanvasLoopCopyCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
		PatternLoopListPtr pPatternLoops_willCopy, const std::vector<LONGLONG>& textStructs_willCopy, const Matrix2D& transformMatrix);
	~CanvasLoopCopyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

public:
	std::tr1::tuple<PatternListPtr, PatternLoopListPtr, TextStructListPtr> GetNewCadData() const { return m_newCadData; }

private:
	// ϣ�������ļ������������ֶ����Լ��������ĸ�λ�á�
	PatternLoopListPtr m_pPatternLoops_willCopy;
	std::vector<LONGLONG> m_textStructs_willCopy;
	Matrix2D m_transformMatrix;

	// �´�����cad���ݡ�
	std::tr1::tuple<PatternListPtr, PatternLoopListPtr, TextStructListPtr> m_newCadData;
};

END_CUTLEADER_NAMESPACE()
