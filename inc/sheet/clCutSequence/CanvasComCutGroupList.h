#pragma once

#include "clCutSequenceCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(CanvasComCutGroup)
DECLARE_CUTLEADER_CLASS(CanvasComCutGroupList)

BEGIN_CUTLEADER_NAMESPACE()

class CanvasComCutGroupList : public std::vector<CanvasComCutGroupPtr>
{
public:
	CanvasComCutGroupList(void);
	~CanvasComCutGroupList(void);

public:
	// �õ�����ĳ�����������ġ������顱��
	CanvasComCutGroupPtr GetComCutGroup(LONGLONG iPatternLoopID) const;
	int GetGroupIndex(LONGLONG iPatternLoopID) const;

	// �õ������顣
	CanvasComCutGroupPtr GetGroupByID(LONGLONG iGroupID) const;

	// ɾ��ĳ���������顱��
	void RemoveGroup(LONGLONG iGroupID);
};

END_CUTLEADER_NAMESPACE()
