#pragma once

#include "ClTaskUICommon.h"
#include "LineArc2DStaticControl.h"
#include "taoresource.h"
#include "CGridListCtrlGroups.h"
#include "CLDialog.h"
#include "SkinManager.h"


DECLARE_CUTLEADER_CLASS(TaskItem)


BEGIN_CUTLEADER_NAMESPACE()

// ����Ի��������༭�����е������Ϣ��
// ע��
//  1) ȷ����Ƚ��������ݺͽ����ϵ����ݣ�����������޸ġ�
class ClTaskUI_Export TaskPartsSettingDlg : public CLDialog
{
	DECLARE_DYNAMIC(TaskPartsSettingDlg)

public:
	TaskPartsSettingDlg(CWnd* pParent = NULL);
	virtual ~TaskPartsSettingDlg();

	enum { IDD = IDD_clTaskUI_EditTaskPartInfo };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnBatchSetting();
	afx_msg void OnPartListchanged(NMHDR* pNMHDR, LRESULT* pResult);

    afx_msg void OnClickOk();
    afx_msg void OnClickCancel();

	DECLARE_MESSAGE_MAP()

public:
    void SetTaskItem(TaskItemPtr pTaskItem) { m_pTaskItem = pTaskItem; }

private:
	// ��ʼ���������񡱵���ʽ��
	void InitPartGridStyle();

	// Ԥ���б���ѡ�е������
	void PreviewPart();

	// �ͷš��������е����ݡ�
	void RlsPartGridData();

private: // for controls
	CGridListCtrlGroups m_tableTaskParts;
	CStatic m_staticPartPreview;

	// use this brush to paint the background.
	CBrush m_brush;

	SkinMFCButton m_btnBatchSetting;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // data member
    // �����Ű�������б��Ƿ��ʼ���ꡣ
    BOOL m_bFinishAddNestPart;

    TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE()
