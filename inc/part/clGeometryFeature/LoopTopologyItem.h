#pragma once

#include "clGeometryFeatureCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItem)

BEGIN_CUTLEADER_NAMESPACE()

// ��ͼ�λ�·�������
class ClGeometryFeature_Export LoopTopologyItem : public ClData
{
public:
	LoopTopologyItem(IPatternLoopPtr pPatternLoop);
	~LoopTopologyItem(void);

public: // get/set functions.
	void SetPatternLoop(IPatternLoopPtr pPatternLoop) { m_pPatternLoop = pPatternLoop;}
	IPatternLoopPtr GetPatternLoop() const { return m_pPatternLoop; }
	const IPatternLoop* GetPatternLoop_FR() const { return m_pPatternLoop.get(); }

	void SetSubItemList(LoopTopologyItemListPtr pSubItemList) { m_pSubItemList = pSubItemList; }
	LoopTopologyItemListPtr GetSubItemList() const { return m_pSubItemList; }

public:
	// �ݹ�õ����������е����м���������
	void GetAllPatternLoops(PatternLoopListPtr pAllPatternLoops) const;

	// �ݹ�õ�������˽ڵ��������������������û���ڲ���������ô����Ϊ1��
	int CalcLayerNum() const;

	// �ݹ�õ�������������һ�㣬���iPatternLoopID���ǵ�ǰ��������ô�����ڵ�0�㣬����Ҳ���������������ͷ���-1��
	int GetLayerIndexOfPatternLoop(long long iPatternLoopID) const;

	// �ݹ��¡������˽ṹ�����������������¡�ô���ġ�
	// ע��
	// 1) m_pPartPlacements�е������pPartPlacements_old�в�һ�������ҵ������Կ�¡����������к�������������ܱ�ԭ��������١�
	LoopTopologyItemPtr CloneRecursively(const PatternLoopList* pPatternLoops_old, const PatternLoopList* pPatternLoops_new) const;

private:
	// ��ǰͼ�λ�·��
	IPatternLoopPtr m_pPatternLoop;

	// �û�·�����ġ��������
	LoopTopologyItemListPtr m_pSubItemList;
};

END_CUTLEADER_NAMESPACE()
