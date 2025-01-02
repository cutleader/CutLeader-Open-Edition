#include "StdAfx.h"
#include "StaticImgEntity.h"

#include "ImgData.h"

BEGIN_CUTLEADER_NAMESPACE()

StaticImgEntity::StaticImgEntity(void)
{
	m_strName = _T("New Static Picture");
}

StaticImgEntity::~StaticImgEntity(void)
{
}

IDataPtr StaticImgEntity::Clone() const
{
	StaticImgEntityPtr pStaticImgEntity(new StaticImgEntity);
	pStaticImgEntity->SetBorderRect(CRect(m_iLeft, m_iTop, m_iRight, m_iBottom));

	// update the border style.
	UpdateBorderStyle(pStaticImgEntity);

	/************************************************************************/
	// clone image data.

	if (m_pImgData)
	{
		ImgDataPtr pImgData(new ImgData);
		pStaticImgEntity->SetImageData(pImgData);

		// the old image data info.
		int iOldSize = m_pImgData->GetSize();
		LPVOID pOldData = m_pImgData->GetData();

		// the new one.
		LPVOID pNewData = malloc(iOldSize);
		memcpy(pNewData, pOldData, iOldSize);
		pImgData->SetData(pNewData);
		pImgData->SetSize(iOldSize);
		pImgData->SetExt(m_pImgData->GetExt());
	}
	/************************************************************************/

	return pStaticImgEntity;
}

END_CUTLEADER_NAMESPACE()
