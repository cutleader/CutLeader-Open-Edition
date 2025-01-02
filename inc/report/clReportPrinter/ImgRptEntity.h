#pragma once

#include "BorderRptEntityBase.h"

DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(ImgRptEntity)

BEGIN_CUTLEADER_NAMESPACE()

// the image entity in the report.
class ImgRptEntity : public BorderRptEntityBase
{
public:
	ImgRptEntity(void);
	~ImgRptEntity(void);

public: // implement IRptEntity interface.
	virtual RPT_ENTITY_TYPE GetType() const { return RPT_ENTITY_IMG; }

public: // set/get functions.
	void SetImageData(ImgDataPtr pImgData) { m_pImgData = pImgData; }
	ImgDataPtr GetImageData() { return m_pImgData; }

private:
	// the image data.
	// note:
	//   1) if this entity do not the image, this will be NULL.
	ImgDataPtr m_pImgData;
};

END_CUTLEADER_NAMESPACE()
