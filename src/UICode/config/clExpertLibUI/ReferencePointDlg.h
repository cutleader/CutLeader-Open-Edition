#pragma once

#include "clExpertLibUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(SequenceConfigItem)

BEGIN_CUTLEADER_NAMESPACE()

class ReferencePointDlg : public CLDialog
{
	DECLARE_DYNCREATE(ReferencePointDlg)

public:
	ReferencePointDlg();
	~ReferencePointDlg(void);

	enum { IDD = IDD_clExpertLibUI_ReferencePt };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	void Init(SequenceConfigItemPtr pSequenceConfigItem) { m_pSequenceConfigItem = pSequenceConfigItem; }

private: // controls
	int m_iCanvasRefPtPosition;
	BOOL m_bExcludeUncutGeometry;
	BOOL m_bCalculateCamFeature;
	int m_iSheetRefPtPosition;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private:
	SequenceConfigItemPtr m_pSequenceConfigItem;
};

END_CUTLEADER_NAMESPACE()
