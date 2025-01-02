#pragma once

#include "ClDataManagerCommon.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "InvCellGrid.h"
#include "SkinManager.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(ImgData)


BEGIN_CUTLEADER_NAMESPACE()


class ClDataManager_Export TaskInfoPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(TaskInfoPage)

public:
	TaskInfoPage();
	~TaskInfoPage();

	enum { IDD = IDD_ClDataManagerUI_TaskInfo };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected: // message mapping.
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnKillTaskName();
	afx_msg void OnKillTaskComment();

    afx_msg void OnSelchangedPart(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchangedSheet(NMHDR* pNMHDR, LRESULT* pResult);

    afx_msg void OnViewSelectedPart();
	afx_msg void OnChangeMaterial();
	afx_msg void OnChangeCamParam();
    afx_msg void OnExportSheetAsDxf();
	afx_msg void OnExportAllSheetsAsDxf();

	DECLARE_MESSAGE_MAP()

public:
	// �����޸Ĺ������ݣ���Ȼ����û���޸ģ���
	// ע��
	// 1) �ú�����Ҫ�ѽ����赽���Ի�����ȷ���༭����ʧȥ���㡣
	// 2�������ڻطŽű�ʱû�и��༭����killFocus��Ϣ���ú�����Ҫ�ѱ༭���ֵ���µ������С�
	TaskItemPtr GetTaskItem();

public:
	void DisplayTaskItem(TaskItemPtr pTaskItem);
	void UpdateModifyDate();

private:
	// init the column header of the list ctrl.
	void InitGridStyle();

	// �ڱ������ʾ���ݡ�
	void InitGridData();

	// release the data which stored in grid.
	void ReleaseGridItemData();

private: // controls
	// basic info group.
	CString m_strName;
	CEdit m_ctrlNameEditBox;
	CString m_strComment;
	CEdit m_ctrlCommentEditBox;
	CString m_strCreateDate;
	CString m_strModifyDate;

	// Ԥ������
	CStatic m_partPreviewWnd;
    CStatic m_sheetPreviewWnd;

	// ���ؼ�
	SkinInvCellGrid m_partGrid;
	InvCellGrid m_sheetGrid;

	SkinMFCButton m_ctrlViewSelectedPartButton;
	SkinMFCButton m_ctrlExportSheetButton;
	SkinMFCButton m_ctrlExportAllSheetsButton;
	SkinMFCButton m_ctrlChangeMaterialButton;
	SkinMFCButton m_ctrlSwitchCAMParamButton;

	// use this brush to paint the background.
	CBrush m_brush;

private: // data member
	TaskItemPtr m_pTaskItem;

    // �Ƿ��ֹ���ġ�ѡ��仯����Ϣ��
    bool m_bDisableSelectChangeMessageOfGrid;

    // Ԥ�����ݡ�
    ImgDataPtr m_pPartImgData;
    ImgDataPtr m_pSheetImgData;
};

END_CUTLEADER_NAMESPACE()
