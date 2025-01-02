#pragma once

#include "clCutSequenceUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(SheetRefPtData)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequenceUI_Export SheetReferencePointDlg : public CLDialog
{
	DECLARE_DYNCREATE(SheetReferencePointDlg)

public:
	SheetReferencePointDlg();
	~SheetReferencePointDlg(void);

	enum { IDD = IDD_clCutSequenceUI_SheetReferencePt };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	void Init(SheetRefPtDataPtr pSheetRefPtData) { m_pSheetRefPtData = pSheetRefPtData; }

private: // controls
	int m_iSheetRefPtPosition;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private:
	SheetRefPtDataPtr m_pSheetRefPtData;
};

END_CUTLEADER_NAMESPACE()
