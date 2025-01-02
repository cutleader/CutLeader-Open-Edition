#pragma once

#include "ClPartCommon.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(Part)
DECLARE_CUTLEADER_CLASS(PartList)

BEGIN_CUTLEADER_NAMESPACE()


// Áã¼þ¼¯ºÏ
class ClPart_Export PartList : public std::vector<PartPtr>
{
public:
	PartList(void);
	~PartList(void);

public:
    PartPtr GetPartByID(LONGLONG iPartID) const;
};

END_CUTLEADER_NAMESPACE()
