#pragma once

#include "ClPartUICommon.h"
#include "baseConst.h"
#include "taoResource.h"
#include "LineArc2DStaticControl.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DxfData)

BEGIN_CUTLEADER_NAMESPACE()

// 这个对话框用于从dxf/dwg文件新建零件。
class ClPartUI_Export NewPartFromDxfDwgDlg : public CFileDialog
{
	DECLARE_DYNAMIC(NewPartFromDxfDwgDlg)

public:
	NewPartFromDxfDwgDlg(CWnd* pParentWnd = NULL);
	virtual ~NewPartFromDxfDwgDlg();

	enum { IDD = IDD_clPartUI_NewPartFromDxfDwg };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnFileNameChange();
	virtual BOOL OnFileNameOK();

protected: // message mapping.
	afx_msg void OnDestroy();
    afx_msg void OnSelectCamParam();

	DECLARE_MESSAGE_MAP()

public:
	// 得到参数配置。
	LONGLONG GetParamConfigID() const { return m_iParamConfigID; }

private: // 控件
	LineArc2DStaticControl m_ctrlPreviewWnd;

private: // 数据
	// 参数配置
    LONGLONG m_iExpLibItemID;
	LONGLONG m_iParamConfigID;
};

END_CUTLEADER_NAMESPACE()
