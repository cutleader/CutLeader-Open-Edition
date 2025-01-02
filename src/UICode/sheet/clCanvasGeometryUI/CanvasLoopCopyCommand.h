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

// 这个命令用来拷贝底图上的轮廓，轮廓上的加工特征也要一起拷贝。
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
	// 希望拷贝的几何轮廓和文字对象，以及拷贝到哪个位置。
	PatternLoopListPtr m_pPatternLoops_willCopy;
	std::vector<LONGLONG> m_textStructs_willCopy;
	Matrix2D m_transformMatrix;

	// 新创建的cad数据。
	std::tr1::tuple<PatternListPtr, PatternLoopListPtr, TextStructListPtr> m_newCadData;
};

END_CUTLEADER_NAMESPACE()
