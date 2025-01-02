#pragma once

#include "clUtilityCommon.h"
#include "TplEntityBase.h"
#include "BindableEntityBase.h"

DECLARE_CUTLEADER_CLASS(BarCodeTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

// 条形码模板实体。
class ClReportTemplate_Export BarCodeTplEntity : virtual public TplEntityBase,
						 virtual public BindableEntityBase
{
public:
	BarCodeTplEntity(void);
	~BarCodeTplEntity(void);

public: // functions from IData interface.
	virtual IDataPtr Clone() const override;

public: // implement ITplEntity interface.
	virtual ENTITY_TYPE GetType() const { return ENTITY_BARCODE; }

public:
	void SetBarCodeType(BARCODE_TYPE iBarCodeType) { m_iBarCodeType = iBarCodeType; }
	BARCODE_TYPE GetBarCodeType() { return m_iBarCodeType; }

	void SetBarCodeContent(CString strBarCodeContent) { m_strBarCodeContent = strBarCodeContent; }
	CString GetBarCodeContent() { return m_strBarCodeContent; }

private:
	// 条形码编码。
	BARCODE_TYPE m_iBarCodeType;

	// 条形码内容。
	// 注：
	//  1) 一般用于测试，如果没有绑定数据就用这个。
	CString m_strBarCodeContent;
};

END_CUTLEADER_NAMESPACE()
