#pragma once

#include "clCanvasGeometryUICommon.h"
#include "taoResource.h"
#include "LineArc2DStaticControl.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DxfData)

BEGIN_CUTLEADER_NAMESPACE()

// 这个对话框用来选择导入到底图的dxf/dwg文件，只能单选。
class ClCanvasGeometryUI_Export ImportDxfDwg2CanvasDlg : public CFileDialog
{
	DECLARE_DYNAMIC(ImportDxfDwg2CanvasDlg)

public:
	ImportDxfDwg2CanvasDlg(CWnd* pParentWnd = NULL);
	virtual ~ImportDxfDwg2CanvasDlg();

	enum { IDD = IDD_clCanvasGeometryUI_ImportDxfDwg2Canvas };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnFileNameChange();
	virtual BOOL OnFileNameOK();

protected: // message mapping.
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

private:
	void DisplayInvalidDxfDwgFileHint();

private: // 控件
	LineArc2DStaticControl m_ctrlPreviewWnd;

private: // 数据
	BOOL m_bInvalidDxfDwgFile;
};

END_CUTLEADER_NAMESPACE()
