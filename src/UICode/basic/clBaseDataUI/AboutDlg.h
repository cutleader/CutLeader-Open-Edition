#pragma once

#include "clBaseDataUICommon.h"
#include "taoresource.h"
#include "XHTMLStatic.h"
#include "XHyperLink.h"
#include "CLDialog.h"

BEGIN_CUTLEADER_NAMESPACE()

// 关于对话框。
class ClBaseDataUI_Export AboutDlg : public CLDialog
{
public:
	AboutDlg();

	enum { IDD = IDD_clBaseDataUI_About };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	DECLARE_MESSAGE_MAP()

private:
	void InitXHTMLStaticStyle( XHTMLStatic *TargetXHTMLStatic );

private:
	CBitmapButton m_btnProductName;

	// app version.
	XHTMLStatic	m_appVersion;

	// 主数据库版本。
	XHTMLStatic	m_dbVersion;

	// 零件版本。
	XHTMLStatic	m_partVersion;

	// 任务版本。
	XHTMLStatic	m_taskVersion;

	// driver version.
	XHTMLStatic	m_driVersion;

	// 专家库版本。
	XHTMLStatic	m_expLibVersion;

	// 材料库版本。
	XHTMLStatic	m_matLibVersion;

	// 报表库版本。
	XHTMLStatic	m_rptLibVersion;
};

END_CUTLEADER_NAMESPACE()
