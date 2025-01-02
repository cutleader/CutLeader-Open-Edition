#pragma once

#include "clOptionUICommon.h"
#include "NumEdit.h"
#include "CLDialog.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ImpExpConfig)

BEGIN_CUTLEADER_NAMESPACE()

class TextDefaultFontDlg : public CLDialog
{
	DECLARE_DYNCREATE(TextDefaultFontDlg)

public:
	TextDefaultFontDlg();
	~TextDefaultFontDlg(void);

	enum { IDD = IDD_clOptionUI_TextDefaultFont };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnSelchangeFontName();
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	void Init(const CString& strFontName, const CString& strFontFileName);
	const CString& GetFontName() const { return m_strFontName; }
	const CString& GetFontFileName() const { return m_strFontFileName; }

private: // controls
	CMFCFontComboBox m_ctrlFontComboBox;
	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private:
	CString m_strFontName;
	CString m_strFontFileName;
};

END_CUTLEADER_NAMESPACE()
