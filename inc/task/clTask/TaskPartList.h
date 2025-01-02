#pragma once

#include "ClTaskCommon.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(TaskPart)
DECLARE_CUTLEADER_CLASS(TaskPartList)


BEGIN_CUTLEADER_NAMESPACE()

class ClTask_Export TaskPartList : public std::vector<TaskPartPtr>
{
public:
	TaskPartList(void);
	~TaskPartList(void);
};

END_CUTLEADER_NAMESPACE()
