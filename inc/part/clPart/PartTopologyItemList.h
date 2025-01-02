#pragma once

#include "clPartCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(PartTopologyItem)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// ���������ϡ�
class ClPart_Export PartTopologyItemList : public std::vector<PartTopologyItemPtr>
{
public:
	PartTopologyItemList(void);
	~PartTopologyItemList(void);

public:
	// �ݹ�õ����������
	// ע��
	//   1) һ������Ԫ�ش���һ�������
	void GetAllParts(std::vector<std::pair<PatternLoopListPtr, LoopTopologyItemListPtr>>& partList);
};

END_CUTLEADER_NAMESPACE()
