#pragma once

#include "ClPartLayoutUICommon.h"
#include "baseConst.h"
#include "taoResource.h"
#include "LineArc2DStaticControl.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DxfData)

BEGIN_CUTLEADER_NAMESPACE()

// 这个对话框用来选择导入到板材的dxf/dwg文件，只能单选。
class ClPartLayoutUI_Export ImportDxfDwg2SheetDlg : public CFileDialog
{
	DECLARE_DYNAMIC(ImportDxfDwg2SheetDlg)

public:
	ImportDxfDwg2SheetDlg(CWnd* pParentWnd = NULL);
	virtual ~ImportDxfDwg2SheetDlg();

	enum { IDD = IDD_clPartLayoutUI_ImportDxfDwg2Sheet };

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
