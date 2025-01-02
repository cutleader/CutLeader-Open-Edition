#pragma once

#include "ClTaskUICommon.h"
#include "baseConst.h"
#include "taoResource.h"
#include "LineArc2DStaticControl.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DxfData)

BEGIN_CUTLEADER_NAMESPACE()

// 这个对话框用来选择导入到任务的dxf/dwg文件。
class ClTaskUI_Export ImportDxfDwg2TaskDlg : public CFileDialog
{
	DECLARE_DYNAMIC(ImportDxfDwg2TaskDlg)

public:
	ImportDxfDwg2TaskDlg(CWnd* pParentWnd = NULL);
	virtual ~ImportDxfDwg2TaskDlg();

	enum { IDD = IDD_clTaskUI_ImportDxfDwg };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnFileNameChange();
	virtual BOOL OnFileNameOK();

protected: // message mapping.
	afx_msg void OnDestroy();
	afx_msg void OnSelectFolder();
    afx_msg void OnSelectCamParam();

	DECLARE_MESSAGE_MAP()

public:
	// 得到保存路径。
	FolderNodePtr GetSavePath() const { return m_pSaveFolder; }

	// 得到参数配置。
	LONGLONG GetParamConfigID() const { return m_iParamConfigID; }

private: // 控件
	LineArc2DStaticControl m_ctrlPreviewWnd;
	CString m_ctrlSavePath;

private: // 数据
	// 保存目录。
	FolderNodePtr m_pSaveFolder;

	// 参数配置
    LONGLONG m_iExpLibItemID;
	LONGLONG m_iParamConfigID;
};

END_CUTLEADER_NAMESPACE()
