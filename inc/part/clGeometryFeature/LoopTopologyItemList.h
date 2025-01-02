#pragma once

#include "clGeometryFeatureCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopTopologyItem)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)

BEGIN_CUTLEADER_NAMESPACE()

// ��ͼ�λ�·��������ϡ�
class ClGeometryFeature_Export LoopTopologyItemList : public std::vector<LoopTopologyItemPtr>
{
public:
	LoopTopologyItemList(void);
	~LoopTopologyItemList(void);

public:
	// ɾ�������
	// ע��
	//   1) ���ǵݹ顣
	void RemoveTopItem(LoopTopologyItemPtr pLoopTopItem);

	// ĳ����������Ƿ���ڡ�
	BOOL TopItemExist(LoopTopologyItemPtr pLoopTopItem);

	// ������˹�ϵ���ǲ������������������
	bool IncludePatternLoop(const IPatternLoop* pPatternLoop) const;

	// ���ݼ�������ID�õ������
	const LoopTopologyItem* FindTopologyItemByPatternLoop(long long iPatternLoopID) const;

	// �õ��������˲���������������ǵݹ麯��������ÿ���ڵ��CalcLayerNum�����ǵݹ麯����
	int CalcLayerNum() const;

	// �õ�������������һ�㣬�������ǵ�0�㣬����Ĳ����Ǵ�0��ʼ�ģ�����Ҳ�����Ҫ��������-1��
	int GetLayerIndexOfPatternLoop(long long iPatternLoopID) const;


	/************************************************************************/
	// ������������
	// ע��
	// 1��������Щ�ӿ�ʱҪע��LoopTopologyItemList������PartCadData������Ǹ����ݡ�

	// �ж�������������ǲ��������������������������������Ǳպϵģ��Ұ�����������������
	bool IsBoundaryLoop(const IPatternLoop* pPatternLoop) const;

	// �õ��������������
	int GetBoundaryLoop(const PatternLoopList* pAllPatternLoops) const;
	IPatternLoopPtr GetBoundaryLoopEx() const;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
