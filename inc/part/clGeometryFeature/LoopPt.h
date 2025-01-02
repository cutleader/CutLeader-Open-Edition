#pragma once

#include "clGeometryFeatureCommon.h"
#include "Point2D.h"

BEGIN_CUTLEADER_NAMESPACE()

// ��¼��������ĵ㡣
// ע��
//  1) Ŀǰ�еĵط�û��������ṹ��
class ClGeometryFeature_Export LoopPt
{
public:
	LoopPt(void);
	LoopPt(const Point2D& loopPt, int iNodeIndex);
	~LoopPt(void);

public: // get/set functions
	void SetLoopPt(const Point2D& pt) { m_loopPt = pt; }
	Point2D GetLoopPt() const { return m_loopPt; }

	void SetNodeIndex(int iIndex) { m_iNodeIndex = iIndex; }
	int GetNodeIndex() const { return m_iNodeIndex; }

private:
	// ��������ĵ㡣
	Point2D m_loopPt;

	// �����ڵĻ�·�ڵ㡣
	int m_iNodeIndex;
};

END_CUTLEADER_NAMESPACE()
