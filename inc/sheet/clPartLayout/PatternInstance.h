#pragma once

#include "clPartLayoutCommon.h"
#include "Point2D.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(PatternLoopNode)
DECLARE_CUTLEADER_CLASS(PatternInstanceCache)
DECLARE_CUTLEADER_CLASS(LoopInstance)

BEGIN_CUTLEADER_NAMESPACE()

// ͼ��ʵ����
// ע��
//   1) �������û�б��������ݿ⡣
class ClPartLayout_Export PatternInstance : public ClData
{
public:
	PatternInstance(void);
	PatternInstance(LoopInstancePtr pLoopInstance, PatternLoopNodePtr pPatternLoopNode);
	~PatternInstance(void);

public: // get/set functions
	void SetLoopInst(LoopInstancePtr pLoopInstance) { m_pLoopInstance = pLoopInstance; }
	LoopInstancePtr GetLoopInstance() const { return m_pLoopInstance; }

	void SetPatternLoopNode(PatternLoopNodePtr pPatternLoopNode) { m_pPatternLoopNode = pPatternLoopNode; }
	PatternLoopNodePtr GetPatternLoopNode() const { return m_pPatternLoopNode; }

public:
	// ���ڻ������ݡ�
	PatternInstanceCachePtr GetCacheData() const { return m_pPatternInstanceCache; }
	void UpdateCache();

	// ���ͼ��ʵ������ľ��롣
	double DistanceTo(const Point2D& pt) const;

private:
	// ͼ��ʵ�����ڵĻ�·ʵ����
	LoopInstancePtr m_pLoopInstance;

	// ����·�ڵ㡱����
    // ע������ڵ㲻һ���Ǽ��������е��Ǹ��ڵ㣬����������и��ͬʱ������ڵ��ǿ�¡�����ġ���������ڵ��е����ݺͼ��������е���һ���ġ�
	PatternLoopNodePtr m_pPatternLoopNode;

	// ��ͼ��ʵ�����Ļ������ݡ�
	PatternInstanceCachePtr m_pPatternInstanceCache;
};

END_CUTLEADER_NAMESPACE()
