#pragma once

#include "ClDataImportExportUICommon.h"
#include "baseConst.h"
#include "taoResource.h"
#include "LineArc2DStaticControl.h"


DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DxfData)


BEGIN_CUTLEADER_NAMESPACE()

// 这个对话框用来选择导入到数据中心的CutLeader零件文件。
class ClDataImportExportUI_Export ImportClPart2DataCenterDlg : public CFileDialog
{
	DECLARE_DYNAMIC(ImportClPart2DataCenterDlg)

public:
	ImportClPart2DataCenterDlg(CWnd* pParentWnd = NULL);
	virtual ~ImportClPart2DataCenterDlg();

	enum { IDD = IDD_clDataImportExportUI_ImportClPart2DataCenter };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnFileNameChange();
	virtual BOOL OnFileNameOK();

protected: // message mapping.
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

private: // 控件
	LineArc2DStaticControl m_ctrlPreviewWnd;
};

END_CUTLEADER_NAMESPACE()
