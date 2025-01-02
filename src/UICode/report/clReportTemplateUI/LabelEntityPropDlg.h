#pragma once

#include "clReportTemplateCommon.h"
#include "NumEdit.h"
#include "ColorStatic.h"
#include "LabelDrawStatic.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class LabelEntityPropDlg : public CLDialog
{
	DECLARE_DYNCREATE(LabelEntityPropDlg)

public:
	LabelEntityPropDlg();
	~LabelEntityPropDlg(void);

	enum { IDD = IDD_clReportTemplateUI_LabelProperty };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnKillLabelWidth();
	afx_msg void OnKillLabelHeight();
	afx_msg void OnKillRowNum();
	afx_msg void OnKillRowGap();
	afx_msg void OnKillColumnNum();
	afx_msg void OnKillColumnGap();
	afx_msg void OnKillPageWidth();
	afx_msg void OnKillPageHeight();
	afx_msg void OnKillTopMargin();
	afx_msg void OnKillBottomMargin();
	afx_msg void OnKillLeftMargin();
	afx_msg void OnKillRightMargin();

	afx_msg void OnBorderColor();
	afx_msg void OnOK();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	void Init(ReportTplPagePtr pReportTplPage, LabelTplEntityPtr pLabelTplEntity);

private:
	// 基本信息。
	CString m_strName;
	CNumEdit m_ctrlWidth;
	CNumEdit m_ctrlHeight;
	
	// for border.
	BOOL m_bBorder;
	CNumEdit m_iBorderThickness;
	CComboBox m_ctrlBorderStyle;
	ColorStatic	m_ctrlBorderColor;

	// 布局。
	CNumEdit m_ctrlRowNum;
	CNumEdit m_ctrlColumnNum;
	CNumEdit m_ctrlRowGap;
	CNumEdit m_ctrlColumnGap;

	// 页面信息。
	CNumEdit m_dPaperWidth;
	CNumEdit m_dPaperHeight;
	CNumEdit m_dLeftMargin;
	CNumEdit m_dRightMargin;
	CNumEdit m_dTopMargin;
	CNumEdit m_dBottomMargin;

	SkinMFCButton m_btnBorderColor;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

	// 标签预览窗口
	LabelDrawStatic m_ctrlLabelPreview;

private:
	// use this brush to paint the background.
	CBrush m_brush;

	ReportTplPagePtr m_pReportTplPage;
	LabelTplEntityPtr m_pLabelTplEntity;
};

END_CUTLEADER_NAMESPACE()
