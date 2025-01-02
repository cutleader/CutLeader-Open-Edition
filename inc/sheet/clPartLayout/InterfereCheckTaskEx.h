#pragma once

#include "clPartLayoutCommon.h"
#include "TaskEx.h"

DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(InterfereCheckTaskEx)

BEGIN_CUTLEADER_NAMESPACE()

// �����������������������������ߺ�ͼ�εĸ��档
class ClPartLayout_Export InterfereCheckTaskEx : public TaskEx
{
public:
	InterfereCheckTaskEx(PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, ParamConfigPtr pParamConfig);
	~InterfereCheckTaskEx(void);

public: // inherit from class TaskEx.
	virtual void Run();

private:
	PartPlacementListPtr m_pPartPmtList;
	PartInstanceListPtr m_pPartInstList;

	ParamConfigPtr m_pParamConfig;
};

END_CUTLEADER_NAMESPACE()
