#pragma once

#include "SkinManager.h"
#include "clDataImportExportUICommon.h"
#include "taoResource.h"


BEGIN_CUTLEADER_NAMESPACE()

class ClDataImportExportUI_Export DataBackupDlg : public CLDialog
{
	DECLARE_DYNCREATE(DataBackupDlg)

public:
	DataBackupDlg();

	~DataBackupDlg();

	enum { IDD = IDD_clDataImportExportUI_DataBackup };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnSelectDir();
	afx_msg void OnBackup();
	afx_msg void OnRecovery();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	void Init(BOOL bBackupDB, BOOL bBackupLog);

private:
	// use this brush to paint the background.
	CBrush m_brush;

	// the message.
	CString m_strMsg;

	// bk option.
	BOOL m_bBackupDB;
	BOOL m_bBackupLog;

	// backup dir.
	CString m_strBackupDir;

	SkinMFCButton m_btnChangeLog;
	SkinMFCButton m_btnBackup;
	SkinMFCButton m_btnRecovery;
	SkinMFCButton m_btnCancel;
};

END_CUTLEADER_NAMESPACE()
