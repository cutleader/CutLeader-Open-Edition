#pragma once

#include "clReportTemplateCommon.h"
#include "BorderTplEntityBase.h"

DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(StaticImgEntity)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export StaticImgEntity : public BorderTplEntityBase
{
public:
	StaticImgEntity(void);
	~StaticImgEntity(void);

public: // functions from IData interface.
	virtual IDataPtr Clone() const override;

public: // implement ITplEntity interface.
	virtual ENTITY_TYPE GetType() const { return ENTITY_STATIC_IMG; }

public: // set/get functions.
	void SetImageData(ImgDataPtr pImgData) { m_pImgData = pImgData; }
	ImgDataPtr GetImageData() { return m_pImgData; }

private:
	// the image data.
	// note:
	//   1) if do not select image to the entity, this will be NULL.
	ImgDataPtr m_pImgData;
};

END_CUTLEADER_NAMESPACE()
