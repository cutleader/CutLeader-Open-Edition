#pragma once

#include "clDxfDwgCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(TextStruct)
DECLARE_CUTLEADER_CLASS(PatternLoopList)

BEGIN_CUTLEADER_NAMESPACE()

// this class defines all data which imported from dxf/dwg file.
class ClDxfDwg_Export DxfData
{
public:
	DxfData(void);
	~DxfData(void);

public: // get/set functions.
	void SetPatternList(PatternListPtr pPatList) { m_pPatList = pPatList; }
	PatternListPtr GetPatternList() const { return m_pPatList; }

    void SetPolygonPatList(PatternListPtr pPatList_polygon) { m_pPatList_polygon = pPatList_polygon; }
    PatternListPtr GetPolygonPatList() const { return m_pPatList_polygon; }

	const std::vector<std::tr1::tuple<TextStructPtr, PatternListPtr, PatternLoopListPtr>>& GetTextData() const { return m_textData; }

public:
	// 更新缓存。
	// 注：
	// 1) 这个函数只需更新m_pPatList和m_pPatList_polygon的缓存。文字的缓存在创建的时候就更新好了。
	void UpdateCache();

	// calc the base geoms.
	LineArc2DListPtr Calculate2DLineArcs() const;

	// 添加一个文字对象。
	void AddText(const std::tr1::tuple<TextStructPtr, PatternListPtr, PatternLoopListPtr>& text) { m_textData.push_back(text); }

private:
	// 除了多边形pattern和文字以外的所有pattern
	PatternListPtr m_pPatList;

	// 多边形pattern，每个多段线\样条曲线都生成一个多边形pattern
	PatternListPtr m_pPatList_polygon;

	// 导入的文字。
	std::vector<std::tr1::tuple<TextStructPtr, PatternListPtr, PatternLoopListPtr>> m_textData;
};

END_CUTLEADER_NAMESPACE()
