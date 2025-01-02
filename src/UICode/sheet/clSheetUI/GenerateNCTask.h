#pragma once

#include "clSheetUICommon.h"
#include "TaskEx.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(CutNodeList)
DECLARE_CUTLEADER_CLASS(NCCodeData)
DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(GenerateNCTask)

BEGIN_CUTLEADER_NAMESPACE()

class ClSheetUI_Export GenerateNCTask : public TaskEx
{
public:
	GenerateNCTask(SheetPtr pSheet, const Point2D& referencePt);
	~GenerateNCTask(void);

public: // inherit from class TaskEx.
	virtual void Run();

public:
	NCCodeDataPtr GetNCCodeData() const { return m_pNCCodeData; }
	CutNodeListPtr GetCutNodeList() const { return m_pCutNodeList; }
    const std::set<LONGLONG>& GetPatternLoops_failedToOffset() const { return m_patternLoops_failedToOffset; }

private:
	SheetPtr m_pSheet;
	Point2D m_referencePt;

	// 所有“切割节点”集合。
	CutNodeListPtr m_pCutNodeList;

	// the NC data.
	NCCodeDataPtr m_pNCCodeData;

    // 此处记录偏移失败的几何轮廓。
    std::set<LONGLONG> m_patternLoops_failedToOffset;
};

END_CUTLEADER_NAMESPACE()
