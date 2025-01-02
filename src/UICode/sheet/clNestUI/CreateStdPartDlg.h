#pragma once

#include "clNestUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(Part)

BEGIN_CUTLEADER_NAMESPACE()

// 新建标准零件
class CreateStdPartDlg : public CLDialog
{
	DECLARE_DYNAMIC(CreateStdPartDlg)

public:
	CreateStdPartDlg(CWnd* pParent = NULL);
	virtual ~CreateStdPartDlg(void);

	enum { IDD = IDD_clNestUI_Create_StdPart };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnSelectFolder();

	afx_msg void OnRectPart();
	afx_msg void OnRoundPart();

	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	void SetConfigParamID(LONGLONG iParamConfigID) { m_iParamConfigID = iParamConfigID; }

	PartPtr GetNewPart() { return m_pNewPart; }
	FolderNodePtr GetSaveFolder() { return m_pSaveFolder; }
	int GetPartCount() { return m_iPartNum; }

private: // controls.
	// 零件名称。
	CString m_strPartName;

	// 保存路径
	CString m_ctrlSavePath;

	// 零件数量。
	CNumEdit m_ctrlPartNum;

	// 矩形形状。
	int m_iPartShape;
	CNumEdit m_ctrlWidth;
	CNumEdit m_ctrlHeight;
	CNumEdit m_ctrlRad;

	SkinMFCButton m_btnSavePath;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // 数据
	// 参数配置。
	LONGLONG m_iParamConfigID;

	// 保存目录。
	FolderNodePtr m_pSaveFolder;

	// 创建的零件对象。
	PartPtr m_pNewPart;

	// 零件数量。
	int m_iPartNum;
};

END_CUTLEADER_NAMESPACE()
