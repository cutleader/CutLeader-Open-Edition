#pragma once

#include "clCutFeatureUICommon.h"
#include "SkinNumEdit.h"
#include "taoresource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// 修改轮廓缺口对话框。该对话框只负责输入值，不参与逻辑。
class ClCutFeatureUI_Export ChangeLoopGapDlg : public CLDialog
{
	DECLARE_DYNCREATE(ChangeLoopGapDlg)

public:
	ChangeLoopGapDlg();
	~ChangeLoopGapDlg(void);

	enum {
		IDD = IDD_clCutFeatureUI_LoopGapModify
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	double GetGapSize() const { return m_dGapSize; }
	double GetMinLoopWidth() const { return m_dMinLoopWidth; }
	double GetMinLoopHeight() const { return m_dMinLoopHeight; }

private: // controls
	SkinNumEdit m_ctrlGapSize;
	SkinNumEdit m_ctrlMinLoopWidth;
	SkinNumEdit m_ctrlMinLoopHeight;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private:
	double m_dGapSize;

	// 最小轮廓尺寸。也就是轮廓尺寸大于这两个值时才设置对应的缺口值。
	double m_dMinLoopWidth;
	double m_dMinLoopHeight;
};

END_CUTLEADER_NAMESPACE()
