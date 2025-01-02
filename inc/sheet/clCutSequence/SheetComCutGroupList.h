#pragma once

#include "clCutSequenceCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(SheetComCutGroup)

BEGIN_CUTLEADER_NAMESPACE()

class SheetComCutGroupList : public std::vector<SheetComCutGroupPtr>
{
public:
	SheetComCutGroupList(void);
	~SheetComCutGroupList(void);

public:
	// 得到包含“iPartInstID”的“共边组”。
	SheetComCutGroupPtr GetComCutGroup(LONGLONG iPartInstID) const;
	int GetGroupIndex(LONGLONG iPartInstID) const;

	// 得到共边组。
	SheetComCutGroupPtr GetGroupByID(LONGLONG iGroupID) const;

	// 删除某个“共边组”。
	void RemoveGroup(LONGLONG iGroupID);
};

END_CUTLEADER_NAMESPACE()
