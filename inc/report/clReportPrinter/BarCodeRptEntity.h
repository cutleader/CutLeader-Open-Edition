#pragma once

#include "RptEntityBase.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(BarCodeRptEntity)

BEGIN_CUTLEADER_NAMESPACE()

// �������롱����ʵ�塣
class ClReportPrinter_Export BarCodeRptEntity : public RptEntityBase
{
public:
	BarCodeRptEntity(void);
	BarCodeRptEntity(const Rect2D& borderRect, ImgDataPtr pImgData);
	~BarCodeRptEntity(void);

public: // implement IRptEntity interface.
	virtual RPT_ENTITY_TYPE GetType() const { return RPT_ENTITY_BARCODE; }

public: // set/get functions.
	void SetImageData(ImgDataPtr pImgData) { m_pImgData = pImgData; }
	ImgDataPtr GetImageData() { return m_pImgData; }

private:
	// ������ͼ�Ρ�
	// ע��
	//  1) ΪNULL�Ͳ�����
	ImgDataPtr m_pImgData;
};

END_CUTLEADER_NAMESPACE()
