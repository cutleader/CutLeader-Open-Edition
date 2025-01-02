#pragma once

#include "ClTaskUICommon.h"
#include "CLResizableDialog.h"
#include "taoResource.h"
#include "NumEdit.h"
#include "XLabel.h"
#include "SkinManager.h"


DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(TaskItem)


BEGIN_CUTLEADER_NAMESPACE()

class TaskSheetInfoDlg : public CLResizableDialog
{
	DECLARE_DYNCREATE(TaskSheetInfoDlg)

public:
	TaskSheetInfoDlg();
	~TaskSheetInfoDlg();

	enum { IDD = IDD_clTaskUI_SheetInfo };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
    virtual void OnOK() { return; }
    virtual void OnCancel() { return; }

protected: // message mapping.
    afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg LRESULT OnMouseLeave(WPARAM, LPARAM);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

    afx_msg void OnSelectSheet();
    afx_msg void OnActiveSheet();

	afx_msg LRESULT OnCharEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKillFocusEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
    SheetPtr GetSheet() const { return m_pSheet; }

public:
    void Init(SheetPtr pSheet, TaskItemPtr pTaskItem);

	// �������ݡ�
	// ע��
	// 1) ����1���ر�ĳ��������¸�������ܻ�����һЩTaskSheetInfoDlg�������غ�Ҫ����TaskSheetInfoDlg�еĹ������ݣ�
	//    ������ܵ��¸ոչرյ����������ڴ��޷��ͷţ��Ӷ�����ڴ�й¶��
	void ResetData();

    bool IsSelected() const;
    void SelectThisSheet();
    void UnSelectThisSheet();

    void DisplayDataOnDialog();

private: // controls
    CStatic m_previewWnd;
	XLabel m_ctrlSheetNameLabel;
    CNumEdit m_countEditBox;

	// ��ǰ�Ի����ǲ��ǻ�ģ���Ļ�����һ����������������뱾�Ի��������Ϊtrue���뿪������Ϊfalse��
	bool m_bThisDialogIsActive;

private: // data member.
    SheetPtr m_pSheet;
    TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE()
