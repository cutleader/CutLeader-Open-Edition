#pragma once

#include "clGeometryFeatureCommon.h"
#include "ClData.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()

// this class define the cad data of the part.
// notes:
//   1) do not save this object to database.
class ClGeometryFeature_Export PartCadData : public ClData
{
public:
	PartCadData(void);
	PartCadData(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems);
	virtual ~PartCadData(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;

public:
	PatternListPtr GetPatternList() const { return m_pPatList; }
	void SetPatternList(PatternListPtr pPatList) { m_pPatList = pPatList; }

	PatternLoopListPtr GetPatternLoopList() const { return m_pPatternLoopList; }
	void SetPatternLoopList(PatternLoopListPtr pPatternLoopList) { m_pPatternLoopList = pPatternLoopList; }

	TextStructListPtr GetTextStructs() const { return m_pTextStructs; }
	void SetTextStructs(TextStructListPtr pTextStructs) { m_pTextStructs = pTextStructs; }

	const LoopTopologyItemList* GetLoopTopologyItems() const { return m_pLoopTopologyItems.get(); }
	LoopTopologyItemListPtr GetLoopTopologyItems_FW() const { return m_pLoopTopologyItems; }
	void SetLoopTopologyItems(LoopTopologyItemListPtr pLoopTopologyItems) { m_pLoopTopologyItems = pLoopTopologyItems; }

public:
	// �õ���ԭ�㵽ͼ�����µ��������
	virtual Vector2D GetVectToOrigin() const;

private:
	// the patterns in the part
	PatternListPtr m_pPatList;

	// the pattern loop list.
	PatternLoopListPtr m_pPatternLoopList;

	// ���ּ��ϡ�
	TextStructListPtr m_pTextStructs;

	// �������������˹�ϵ��
	// ע��
	// 1��������ݲ���������ݿ⣬�Ǹ��ݼ���������������ġ��������ʵ���ʱ����Ҫ�ǵø��������������Ϊ��������(cache)��
	// 2�������������ʱ���õ��˼��������еĻ��档
	LoopTopologyItemListPtr m_pLoopTopologyItems;
};

END_CUTLEADER_NAMESPACE()
