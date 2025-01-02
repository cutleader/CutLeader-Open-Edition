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
	// �õ�������iPartInstID���ġ������顱��
	SheetComCutGroupPtr GetComCutGroup(LONGLONG iPartInstID) const;
	int GetGroupIndex(LONGLONG iPartInstID) const;

	// �õ������顣
	SheetComCutGroupPtr GetGroupByID(LONGLONG iGroupID) const;

	// ɾ��ĳ���������顱��
	void RemoveGroup(LONGLONG iGroupID);
};

END_CUTLEADER_NAMESPACE()
