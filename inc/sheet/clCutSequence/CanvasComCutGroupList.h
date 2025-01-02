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
	// 得到包含某个几何轮廓的“共边组”。
	CanvasComCutGroupPtr GetComCutGroup(LONGLONG iPatternLoopID) const;
	int GetGroupIndex(LONGLONG iPatternLoopID) const;

	// 得到共边组。
	CanvasComCutGroupPtr GetGroupByID(LONGLONG iGroupID) const;

	// 删除某个“共边组”。
	void RemoveGroup(LONGLONG iGroupID);
};

END_CUTLEADER_NAMESPACE()
