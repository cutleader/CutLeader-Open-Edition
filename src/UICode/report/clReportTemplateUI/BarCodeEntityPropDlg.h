#pragma once

#include "clReportTemplateCommon.h"
#include "taoResource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ExtPropTypeList)
DECLARE_CUTLEADER_CLASS(BarCodeTplEntity)
DECLARE_CUTLEADER_CLASS(BindItemList)

BEGIN_CUTLEADER_NAMESPACE()

class BarCodeEntityPropDlg : public CLDialog
{
	DECLARE_DYNCREATE(BarCodeEntityPropDlg)

public:
	BarCodeEntityPropDlg();
	~BarCodeEntityPropDlg(void);

	enum { IDD = IDD_clReportTemplateUI_BarCodeProperty };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnOK();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	void Init(BarCodeTplEntityPtr pBarCodeTplEnt, BindItemListPtr pAvailBindItemList, ExtPropTypeListPtr pExtPropTypes);

private: // controls.
	CString m_strName;
	CString m_strBarCodeContent;
	CComboBox m_ctrlBarCodeType;

	// 数据绑定。
	int m_iDataBind;
	CComboBox m_ctrlBindType;
	CComboBox m_ctrlExtProp;

	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private:
	BarCodeTplEntityPtr m_pBarCodeTplEnt;
	BindItemListPtr m_pAvailBindItemList;
	ExtPropTypeListPtr m_pExtPropTypes;
};

END_CUTLEADER_NAMESPACE()
