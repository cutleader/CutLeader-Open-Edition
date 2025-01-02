#pragma once

#include "clCanvasGeometryUICommon.h"
#include "CanvasPatternEditCommandBase.h"
#include "Matrix2D.h"
#include "CanvasGeometryHelper.h"

DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)

BEGIN_CUTLEADER_NAMESPACE()

// ��������öԵ�ͼ�ϵ������������У������ϵļӹ�����ҲҪһ�𿽱���
class CanvasLoopGridCommand : public CanvasPatternEditCommandBase
{
public:
	CanvasLoopGridCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
		PatternLoopListPtr pPatternLoops_willCopy, const std::vector<LONGLONG>& textStructs_willCopy, const GeometryGridParam* pGeometryGridParam);
	~CanvasLoopGridCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// ϣ�������еļ������������ֶ����Լ����в�����
	PatternLoopListPtr m_pPatternLoops_willCopy;
	std::vector<LONGLONG> m_textStructs_willCopy;
	const GeometryGridParam* m_pGeometryGridParam;
};

END_CUTLEADER_NAMESPACE()
