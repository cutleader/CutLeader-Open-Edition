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
	// ���»��档
	// ע��
	// 1) �������ֻ�����m_pPatList��m_pPatList_polygon�Ļ��档���ֵĻ����ڴ�����ʱ��͸��º��ˡ�
	void UpdateCache();

	// calc the base geoms.
	LineArc2DListPtr Calculate2DLineArcs() const;

	// ���һ�����ֶ���
	void AddText(const std::tr1::tuple<TextStructPtr, PatternListPtr, PatternLoopListPtr>& text) { m_textData.push_back(text); }

private:
	// ���˶����pattern���������������pattern
	PatternListPtr m_pPatList;

	// �����pattern��ÿ�������\�������߶�����һ�������pattern
	PatternListPtr m_pPatList_polygon;

	// ��������֡�
	std::vector<std::tr1::tuple<TextStructPtr, PatternListPtr, PatternLoopListPtr>> m_textData;
};

END_CUTLEADER_NAMESPACE()
