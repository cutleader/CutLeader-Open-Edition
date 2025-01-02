#pragma once

#include "clSheetUICommon.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "SequenceSetPage.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(Sheet)

BEGIN_CUTLEADER_NAMESPACE()

// use this dialog to set sequence param and reset all sequences of the sheet.
class ClSheetUI_Export ResetSequenceDlg : public CLResizableDialog
{
public:
	ResetSequenceDlg(CWnd* pParent = NULL);
	~ResetSequenceDlg();

	enum { IDD = IDD_clSheetUI_ResetSequence };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	DECLARE_MESSAGE_MAP()

public:
    void Init(SheetPtr pSheet);

private:
    void PlaceSubDlg();
	void SaveParam();

private: // for UI.
	SequenceSetPage m_sequenceSetPage;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // for data
	SheetPtr m_pSheet;
};

END_CUTLEADER_NAMESPACE()
