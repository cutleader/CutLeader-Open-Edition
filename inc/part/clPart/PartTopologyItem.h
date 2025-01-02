#pragma once

#include "clPartCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PartTopologyItemList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// ��������
class ClPart_Export PartTopologyItem : public ClData
{
public:
	PartTopologyItem(void);
	~PartTopologyItem(void);

public: // get/set functions.
	void SetPatternLoops(PatternLoopListPtr pPatternLoops) { m_pPatternLoops = pPatternLoops; }
	PatternLoopListPtr GetPatternLoops() const { return m_pPatternLoops; }

	void SetLoopTopologyItems(LoopTopologyItemListPtr pLoopTopologyItems) { m_pLoopTopologyItems = pLoopTopologyItems; }
	LoopTopologyItemListPtr GetLoopTopologyItems() const { return m_pLoopTopologyItems; }

	void SetSubItems(PartTopologyItemListPtr pSubItems) { m_pSubItems = pSubItems; }
	PartTopologyItemListPtr GetSubItems() const { return m_pSubItems; }

private:
	// ����ļ���������
	PatternLoopListPtr m_pPatternLoops;

	// �����������֮������˹�ϵ��
	LoopTopologyItemListPtr m_pLoopTopologyItems;

	// ����������ġ��������
	PartTopologyItemListPtr m_pSubItems;
};

END_CUTLEADER_NAMESPACE()
