#pragma once

#include "clExpertLibUICommon.h"
#include "NumEdit.h"
#include "Rect2D.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ComCutParam)

BEGIN_CUTLEADER_NAMESPACE()

class ComCutParamDlg : public CLDialog
{
	DECLARE_DYNCREATE(ComCutParamDlg)

public:
	ComCutParamDlg();
	ComCutParamDlg(ComCutParamPtr pComCutParam);
	~ComCutParamDlg(void);

	enum { IDD = IDD_clExpertLibUI_CommonCut_Config };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

private:
	// display the common cutting param.
	void DisplayComCutParam();

	// init the common cutting direction.
	void InitComCutDir(DIRECTION cutDir);

private: // controls
	CComboBox m_cutDir;
	CNumEdit m_dOverCut;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private:
	ComCutParamPtr m_pComCutParam;
};

END_CUTLEADER_NAMESPACE()
