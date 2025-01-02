#pragma once

#include "clFrameCommon.h"
#include "CLDialog.h"
#include "taoresource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class Return2ForegroundCamBar : public CLDialog
{
	DECLARE_DYNCREATE(Return2ForegroundCamBar)

public:
	Return2ForegroundCamBar(CWnd* pParent = NULL);
	~Return2ForegroundCamBar(void);

	enum { IDD = IDD_clFrame_Return2ForegroundCam };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
    afx_msg void OnReturn2ForegroundCam_cancelEdit();
    afx_msg void OnReturn2ForegroundCam_finishEdit();

	DECLARE_MESSAGE_MAP()

private:
	SkinMFCButton m_btn_Return2ForegroundCam_cancelEdit;
	SkinMFCButton m_btn_Return2ForegroundCam_finishEdit;
};

END_CUTLEADER_NAMESPACE()
