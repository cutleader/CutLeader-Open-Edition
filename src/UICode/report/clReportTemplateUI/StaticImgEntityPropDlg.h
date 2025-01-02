#pragma once

#include "clReportTemplateCommon.h"
#include "ColorStatic.h"
#include "taoResource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(StaticImgEntity)

BEGIN_CUTLEADER_NAMESPACE()

class StaticImgEntityPropDlg : public CLDialog
{
	DECLARE_DYNCREATE(StaticImgEntityPropDlg)

public:
	StaticImgEntityPropDlg();
	~StaticImgEntityPropDlg(void);

	enum { IDD = IDD_clReportTemplateUI_StaticPicture_Property };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnPaint();

	afx_msg void OnBrowse();
	afx_msg void OnBorderColor();
	afx_msg void OnOK();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	void Init(StaticImgEntityPtr pStaticImgEntity) { m_pStaticImgEntity = pStaticImgEntity; }

private: // for controls.
	CString m_strName;
	CStatic m_ctrlImgPreview;

	// for border.
	BOOL m_bBorder;
	int m_iBorderThickness;
	CComboBox m_ctrlBorderStyle;
	ColorStatic	m_ctrlBorderColor;

	SkinMFCButton m_btnBorderColor;
	SkinMFCButton m_btnBrowse;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private:
	StaticImgEntityPtr m_pStaticImgEntity;

	// if select image files, these variables will be set.
	CString m_strImgFilePath;
	CString m_strImgFileExt;
};

END_CUTLEADER_NAMESPACE()
