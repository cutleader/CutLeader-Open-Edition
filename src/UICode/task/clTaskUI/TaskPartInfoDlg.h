#pragma once

#include "ClTaskUICommon.h"
#include "CLResizableDialog.h"
#include "taoResource.h"
#include "NumEdit.h"
#include "XLabel.h"
#include "SkinManager.h"


DECLARE_CUTLEADER_CLASS(TaskPart)
DECLARE_CUTLEADER_CLASS(TaskItem)


BEGIN_CUTLEADER_NAMESPACE()

class TaskPartInfoDlg : public CLResizableDialog
{
	DECLARE_DYNCREATE(TaskPartInfoDlg)

public:
	TaskPartInfoDlg();
	~TaskPartInfoDlg();

	enum { IDD = IDD_clTaskUI_TaskPartInfo };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
    virtual void OnOK() { return; }
    virtual void OnCancel() { return; }

protected: // message mapping.
    afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg LRESULT OnMouseLeave(WPARAM, LPARAM);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

    afx_msg void OnSelectTaskPart();
    afx_msg void OnPlaceToSheet();
	afx_msg void OnInplaceEditPart();

	afx_msg LRESULT OnCharEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKillFocusEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
    TaskPartPtr GetTaskPart() const { return m_pTaskPart; }

public:
    void Init(TaskPartPtr pTaskPart, TaskItemPtr pTaskItem);

	// �������ݡ�
	// ע��
	// 1) ����1���ر�ĳ��������¸�������ܻ�����һЩTaskPartInfoDlg�������غ�Ҫ����TaskPartInfoDlg�еĹ������ݣ�
	//    ������ܵ��¸ոչرյ����������ڴ��޷��ͷţ��Ӷ�����ڴ�й¶��
	void ResetData();

    void DisplayDataOnDialog();

	// ˢ����ʾ������������������������ȽϽ����ϵ����ֺ����ݽṹ�е���ֵ��ֻ������ʾ�仯����ֵ��
	void ReDisplayNestedCount();

    bool IsSelected() const;
    void SelectThisTaskPart();
    void UnSelectThisTaskPart();

private: // controls
	CBrush m_brush;
    CStatic m_previewWnd;
	XLabel m_ctrlPartNameLabel;
    XLabel m_nestedCountLabel;
    CNumEdit m_initialCountEditBox;
    SkinMFCButton m_btnPlacePart;

private: // data member.
    TaskPartPtr m_pTaskPart;
    TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE()
