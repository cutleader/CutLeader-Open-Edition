#pragma once

#include "clCutSequenceUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(CanvasRefPtData)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequenceUI_Export CanvasReferencePointDlg : public CLDialog
{
	DECLARE_DYNCREATE(CanvasReferencePointDlg)

public:
	CanvasReferencePointDlg();
	~CanvasReferencePointDlg(void);

	enum { IDD = IDD_clCutSequenceUI_CanvasReferencePt };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	void Init(CanvasRefPtDataPtr pCanvasRefPtData) { m_pCanvasRefPtData = pCanvasRefPtData; }

private: // controls
	int m_iCanvasRefPtPosition;
	BOOL m_bExcludeUncutGeometry;
	BOOL m_bCalculateCamFeature;
	int m_iSheetRefPtPosition;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private:
	CanvasRefPtDataPtr m_pCanvasRefPtData;
};

END_CUTLEADER_NAMESPACE()
