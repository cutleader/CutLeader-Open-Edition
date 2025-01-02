#pragma once

#include "clCanvasGeometryUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "SkinManager.h"
#include "CLDialog.h"
#include "LineArc2DStaticControl.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(PatternList)

BEGIN_CUTLEADER_NAMESPACE()

// 这个对话框主要进行一些零件的设置，不会进行零件的创建和保存工作。
class ClCanvasGeometryUI_Export CreatePartFromGeometryDlg : public CLDialog
{
	DECLARE_DYNAMIC(CreatePartFromGeometryDlg)

public:
	CreatePartFromGeometryDlg(const PatternList* pPatterns, CWnd* pParent = NULL);
	virtual ~CreatePartFromGeometryDlg(void);

	enum { IDD = IDD_clCanvasGeometryUI_CreatePartFromGeometry };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSelectFolder();
	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	const CString& GetPartName() const { return m_strPartName; }
	FolderNodePtr GetSaveFolder() const { return m_pSaveFolder; }
	int GetPartCount() { return (int)m_ctrlPartNum.Get_Number(); }

private: // controls.
	CString m_strPartName;
	CString m_ctrlSavePath;
	CNumEdit m_ctrlPartNum;
	LineArc2DStaticControl m_patternPreviewWnd;

	SkinMFCButton m_btnSavePath;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // 数据
	// 保存目录。
	FolderNodePtr m_pSaveFolder;

	const PatternList* m_pPatterns;
};

END_CUTLEADER_NAMESPACE()
