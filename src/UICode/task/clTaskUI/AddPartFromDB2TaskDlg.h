#pragma once

#include "ClTaskUICommon.h"
#include "taoresource.h"
#include "CGridListCtrlGroups.h"
#include "SkinManager.h"


DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(TaskItem)

BEGIN_CUTLEADER_NAMESPACE()

class ClTaskUI_Export AddPartFromDB2TaskDlg : public CLDialog
{
	DECLARE_DYNAMIC(AddPartFromDB2TaskDlg)

public:
	AddPartFromDB2TaskDlg(CWnd* pParent = NULL);
	virtual ~AddPartFromDB2TaskDlg();

	enum { IDD = IDD_clTaskUI_AddPartFromDB2Task };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();

	afx_msg void OnCheckFolderFilter();
	afx_msg void OnCheckNameFilter();
	afx_msg void OnSelectFilterFolder();
	afx_msg void OnSearch();

	afx_msg void OnPartListchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectAll();

	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	void Init(FolderNodePtr pWorkspace) { m_pFilterFolder = pWorkspace; }

	// �õ�ѡ�е������
	IntegerListPtr GetSelectedPart() { return m_pSelPartIDList; }

    void SetTaskItem(TaskItemPtr pTaskItem) { m_pTaskItem = pTaskItem; }

private:
	// ��ʼ�������ʽ��
	void InitListStyle();

	// ��ʼ��������ݡ�
	void InitListData();

	// �ͷű���е����ݡ�
	void ReleaseGridData();

	// Ԥ���б���ѡ�е������
	void PreviewPart();

private: // for controls
	// Ŀ¼��
	BOOL m_bFolderFilter;

	// ����
	BOOL m_bNameFilter;
	CString m_strNameFilter;

	// ����б�
	CGridListCtrlGroups m_partList;

	// ���Ԥ�����ڡ�
	CStatic m_partPreviewWnd;

	// �Ƿ�ȫѡ��
	BOOL m_bSelectAll;

	SkinMFCButton m_btnSearch;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // data member
	// ������λ�á�
	FolderNodePtr m_pFilterFolder;

	// use this brush to paint the background.
	CBrush m_brush;

	// ����б��Ƿ��ʼ���ꡣ
	BOOL m_bFinishAddPart;

	// ѡ�е������
	IntegerListPtr m_pSelPartIDList;

    TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE()
