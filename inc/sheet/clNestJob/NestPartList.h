#pragma once

#include "clNestJobCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(NestPart)

BEGIN_CUTLEADER_NAMESPACE()

// ��������������ϡ�
class ClNestJob_Export NestPartList : public std::vector<NestPartPtr>
{
public:
	NestPartList(void);
	~NestPartList(void);

public:
	NestPartPtr GetNestPart(LONGLONG iPartID);

	void DeleteNestPart(LONGLONG iPartID);

	BOOL NestPartExist(LONGLONG iPartID);
};

END_CUTLEADER_NAMESPACE()
