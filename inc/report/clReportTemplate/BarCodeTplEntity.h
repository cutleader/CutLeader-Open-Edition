#pragma once

#include "clUtilityCommon.h"
#include "TplEntityBase.h"
#include "BindableEntityBase.h"

DECLARE_CUTLEADER_CLASS(BarCodeTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

// ������ģ��ʵ�塣
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
	// ��������롣
	BARCODE_TYPE m_iBarCodeType;

	// ���������ݡ�
	// ע��
	//  1) һ�����ڲ��ԣ����û�а����ݾ��������
	CString m_strBarCodeContent;
};

END_CUTLEADER_NAMESPACE()
