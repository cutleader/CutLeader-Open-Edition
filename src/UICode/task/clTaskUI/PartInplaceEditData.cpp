#include "StdAfx.h"
#include "PartInplaceEditData.h"

#include "baseConst.h"

BEGIN_CUTLEADER_NAMESPACE()

PartInplaceEditData* PartInplaceEditData::m_pPartInplaceEditManager = NULL;

PartInplaceEditData::PartInplaceEditData(void)
{
	m_bUnderInPlaceEditMode = false;
}

PartInplaceEditData::~PartInplaceEditData(void)
{
}

PartInplaceEditData* PartInplaceEditData::GetInstance()
{
	if (m_pPartInplaceEditManager == NULL)
		m_pPartInplaceEditManager = new PartInplaceEditData();

	return m_pPartInplaceEditManager;
}

void PartInplaceEditData::InitInplaceEditData(int iTaskTabIndex, const PartPtr pPart, const PartCamDataPtr pPartCamData)
{
	m_bUnderInPlaceEditMode = true;
	m_iTaskTabIndex = iTaskTabIndex;
	m_pPart = pPart;
	m_pPartCamData = pPartCamData;
}

void PartInplaceEditData::ResetInplaceEditData()
{
	m_bUnderInPlaceEditMode = false;
	m_iTaskTabIndex = INVALID_IDX;
	m_pPart.reset();
	m_pPartCamData.reset();
}

END_CUTLEADER_NAMESPACE()
