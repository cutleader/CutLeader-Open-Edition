#pragma once

#include "clCanvasGeometryUICommon.h"
#include "CanvasPatternEditCommandBase.h"
#include "Matrix2D.h"
#include "CanvasGeometryHelper.h"

DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)

BEGIN_CUTLEADER_NAMESPACE()

// 这个命令用对底图上的轮廓进行阵列，轮廓上的加工特征也要一起拷贝。
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
	// 希望做阵列的几何轮廓和文字对象，以及阵列参数。
	PatternLoopListPtr m_pPatternLoops_willCopy;
	std::vector<LONGLONG> m_textStructs_willCopy;
	const GeometryGridParam* m_pGeometryGridParam;
};

END_CUTLEADER_NAMESPACE()
