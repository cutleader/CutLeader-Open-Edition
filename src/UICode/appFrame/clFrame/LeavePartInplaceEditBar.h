#pragma once

#include "clFrameCommon.h"
#include "CLDialog.h"
#include "taoresource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// “离开在位编辑零件”面板。
class LeavePartInplaceEditBar : public CLDialog
{
	DECLARE_DYNCREATE(LeavePartInplaceEditBar)

public:
	LeavePartInplaceEditBar(CWnd* pParent = NULL);
	~LeavePartInplaceEditBar(void);

	enum { IDD = IDD_clFrame_LeavePartInplaceEdit };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
    afx_msg void OnCancelEdit();
    afx_msg void OnFinishEdit();

	DECLARE_MESSAGE_MAP()

private:
	SkinMFCButton m_btnCancelEdit;
	SkinMFCButton m_btnFinishEdit;
};

END_CUTLEADER_NAMESPACE()
