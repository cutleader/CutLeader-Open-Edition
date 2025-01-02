#pragma once

#include "clMaterialLibUICommon.h"
#include "LineArc2DStaticControl.h"

BEGIN_CUTLEADER_NAMESPACE()

// 这个对话框用来导入材料。
class ClMaterialLibUI_Export ImportMaterialDlg : public CFileDialog
{
	DECLARE_DYNAMIC(ImportMaterialDlg)

public:
	ImportMaterialDlg(DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR lpszFilter = NULL, CWnd* pParentWnd = NULL);
	virtual ~ImportMaterialDlg();

protected:
	virtual BOOL OnInitDialog();
	virtual void OnFileNameChange();
	virtual BOOL OnFileNameOK();

protected:
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

private: // for control
	LineArc2DStaticControl m_previewStatic;
};

END_CUTLEADER_NAMESPACE()
