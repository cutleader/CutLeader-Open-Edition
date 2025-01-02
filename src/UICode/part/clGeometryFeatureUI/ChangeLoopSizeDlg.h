#pragma once

#include "clGeometryFeatureUICommon.h"
#include "SkinNumEdit.h"
#include "taoresource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// 修改轮廓尺寸对话框。该对话框只负责输入值，不参与逻辑。
class ClGeometryFeatureUI_Export ChangeLoopSizeDlg : public CLDialog
{
	DECLARE_DYNCREATE(ChangeLoopSizeDlg)

public:
	ChangeLoopSizeDlg();
	~ChangeLoopSizeDlg(void);

	enum { IDD = IDD_clGeometryFeatureUI_ChangeLoopSize };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	void SetSize(double dLoopWidth, double dLoopHeight);
	void GetSize(double& dLoopWidth, double& dLoopHeight) const;

private: // controls
	SkinNumEdit m_ctrlLoopWidth;
	SkinNumEdit m_ctrlLoopHeight;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private:
	double m_dLoopWidth;
	double m_dLoopHeight;
};

END_CUTLEADER_NAMESPACE()
