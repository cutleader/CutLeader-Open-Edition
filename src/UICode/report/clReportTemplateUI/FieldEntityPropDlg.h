#pragma once

#include "clReportTemplateCommon.h"
#include "taoResource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ExtPropTypeList)
DECLARE_CUTLEADER_CLASS(BindItemList)
DECLARE_CUTLEADER_CLASS(FieldTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

class FieldEntityPropDlg : public CLDialog
{
	DECLARE_DYNCREATE(FieldEntityPropDlg)

public:
	FieldEntityPropDlg();
	~FieldEntityPropDlg(void);

	enum { IDD = IDD_clReportTemplateUI_Field_Property };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnFont();
	afx_msg void OnOK();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	void Init(FieldTplEntityPtr pFieldTplEntity, BindItemListPtr pAvailBindItemList, ExtPropTypeListPtr pExtPropTypes);

private: // controls.
	CString m_strName;
	int m_iJustification;

	// 数据绑定。
	int m_iDataBind;
	CComboBox m_ctrlBindType;
	CComboBox m_ctrlExtProp;

	SkinMFCButton m_btnFont;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private:
	FieldTplEntityPtr m_pFieldTplEntity;
	BindItemListPtr m_pAvailBindItemList;
	ExtPropTypeListPtr m_pExtPropTypes;

	// font info.
	LOGFONT	m_logicFont;
	COLORREF m_fontColor;
};

END_CUTLEADER_NAMESPACE()
