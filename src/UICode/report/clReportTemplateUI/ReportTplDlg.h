#pragma once

#include "clReportTemplateUICommon.h"
#include "NumEdit.h"
#include "taoResource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ReportTpl)
DECLARE_CUTLEADER_CLASS(ReportTplList)
DECLARE_CUTLEADER_CLASS(ReportTplPage)
DECLARE_CUTLEADER_CLASS(ReportTplPageList)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplateUI_Export ReportTplDlg : public CLDialog
{
	DECLARE_DYNCREATE(ReportTplDlg)

public:
	ReportTplDlg(void);
	~ReportTplDlg(void);

	enum { IDD = IDD_clReportConfigUI_ReportTemplate };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnSelchangeTplType();
	afx_msg void OnSelchangeTpl();

	afx_msg void OnAddTpl();
	afx_msg void OnDelTpl();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDown();

	afx_msg void OnAddPage();
	afx_msg void OnDelPage();

	afx_msg void OnApply();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

private:
	void InitListBox(ReportTplListPtr pReportTplList);

	// init the template info based on the selected template.
	void InitTplInfo();

	// insert one row to the grid.
	void InsertGridRow(int iRowIndex, ReportTplPagePtr pReportTplPage);

	// check whether the template is modified.
	BOOL IsTplModified();

	// save the change to database.
	void SaveChange();

private: // controls.
	CComboBox m_ctrlTplType;
	CListBox m_ctrlTplList;

	// basic info.
	CString m_strTplName;
	CString m_strTplType;
	CString m_strCreateDate;
	CString m_strModifyDate;
	CString m_strComment;

	// template page list.
	SkinGridListCtrlGroups m_ctrlTplPageList;

	SkinMFCButton m_btnAdd;
	SkinMFCButton m_btnDel;
	SkinMFCButton m_btnAddPage;
	SkinMFCButton m_btnDelPage;
	SkinMFCButton m_btnMoveUp;
	SkinMFCButton m_btnMoveDown;
	SkinMFCButton m_btnApply;
	SkinMFCButton m_btnCancel;

private:
	// all report template, do not load page entities.
	ReportTplListPtr m_pReportTplList;

	// template which display currently.
	ReportTplPtr m_pReportTpl;

	// when init "m_pReportTpl", copy each page into this variable.
	ReportTplPageListPtr m_pTmpTplPageList;
};

END_CUTLEADER_NAMESPACE()
