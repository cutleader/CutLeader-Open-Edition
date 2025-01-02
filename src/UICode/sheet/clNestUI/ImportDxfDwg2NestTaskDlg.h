#pragma once

#include "ClNestUICommon.h"
#include "baseConst.h"
#include "taoResource.h"
#include "LineArc2DStaticControl.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DxfData)

BEGIN_CUTLEADER_NAMESPACE()

// 这个对话框用来选择导入到排版任务的dxf/dwg文件，可以多选。
class ClNestUI_Export ImportDxfDwg2NestTaskDlg : public CFileDialog
{
	DECLARE_DYNAMIC(ImportDxfDwg2NestTaskDlg)

public:
	ImportDxfDwg2NestTaskDlg(CWnd* pParentWnd = NULL);
	virtual ~ImportDxfDwg2NestTaskDlg();

	enum { IDD = IDD_clNestUI_ImportDxfDwg2NestTask };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnFileNameChange();
	virtual BOOL OnFileNameOK();

protected: // message mapping.
	afx_msg void OnDestroy();
	afx_msg void OnSelectFolder();

	DECLARE_MESSAGE_MAP()

public:
	// 得到保存路径。
	FolderNodePtr GetSavePath() const { return m_pSaveFolder; }

private: // 控件
	LineArc2DStaticControl m_ctrlPreviewWnd;
	CString m_ctrlSavePath;

private: // 数据
	// 保存目录。
	FolderNodePtr m_pSaveFolder;
};

END_CUTLEADER_NAMESPACE()
