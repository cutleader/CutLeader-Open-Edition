#pragma once

#include "clPartLayoutUICommon.h"
#include "afxwin.h"
#include "SkinNumEdit.h"
#include "taoResource.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(Sheet)

BEGIN_CUTLEADER_NAMESPACE()

// 修改轮廓刀具的对话框。
// 注：
// 1) 该对话框一直浮动在板材视图的右侧，这个对话框是动作发起者。
// 2) 这个对话框对应到一个板材（要读取板材对应的专家库），所以当切换板材时需要更新这个对话框。
class ClPartLayoutUI_Export ChangeLoopToolDlg : public CLResizableDialog
{
	DECLARE_DYNCREATE(ChangeLoopToolDlg)

public:
	ChangeLoopToolDlg();
	~ChangeLoopToolDlg();

	enum { IDD = IDD_clPartLayoutUI_ChangeLoopTool };

public:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpdis);
	afx_msg void OnDestroy();

	afx_msg void OnNoCutLoopInstances();
    afx_msg void OnCutLoopInstances();
    afx_msg void OnSetTool_toSelectedLoopInstances();

	DECLARE_MESSAGE_MAP()

public:
    // 对面板进行一些初始化。
    void Init(LONGLONG iParamConfigIDOfCurrentSheet);

private:
	// 刀具按钮。
	SkinMFCButton m_noCutLoopInstancesButton;
    SkinMFCButton m_cutLoopInstancesButton;
	SkinMFCButton m_toolButton_1;
	SkinMFCButton m_toolButton_2;
	SkinMFCButton m_toolButton_3;
	SkinMFCButton m_toolButton_4;
	SkinMFCButton m_toolButton_5;
	SkinMFCButton m_toolButton_6;
	SkinMFCButton m_toolButton_7;
	SkinMFCButton m_toolButton_8;
	SkinMFCButton m_toolButton_9;
	SkinMFCButton m_toolButton_10;
	std::vector<SkinMFCButton*> m_allToolButton;

private:
	LONGLONG m_iParamConfigIDOfCurrentSheet;
};

END_CUTLEADER_NAMESPACE()
