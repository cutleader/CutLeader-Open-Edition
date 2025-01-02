#pragma once

#include "ClTaskUICommon.h"
#include "baseConst.h"
#include "taoResource.h"
#include "LineArc2DStaticControl.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DxfData)

BEGIN_CUTLEADER_NAMESPACE()

// ����Ի�������ѡ���뵽�����dxf/dwg�ļ���
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
	// �õ�����·����
	FolderNodePtr GetSavePath() const { return m_pSaveFolder; }

	// �õ��������á�
	LONGLONG GetParamConfigID() const { return m_iParamConfigID; }

private: // �ؼ�
	LineArc2DStaticControl m_ctrlPreviewWnd;
	CString m_ctrlSavePath;

private: // ����
	// ����Ŀ¼��
	FolderNodePtr m_pSaveFolder;

	// ��������
    LONGLONG m_iExpLibItemID;
	LONGLONG m_iParamConfigID;
};

END_CUTLEADER_NAMESPACE()
