#include "StdAfx.h"
#include "PartLayoutData.h"

BEGIN_CUTLEADER_NAMESPACE()

PartLayoutData::PartLayoutData(PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances)
{
	m_pPartPmtList = pPartPlacements;
	m_pPartInstList = pPartInstances;
}

PartLayoutData::~PartLayoutData(void)
{
}

END_CUTLEADER_NAMESPACE()
