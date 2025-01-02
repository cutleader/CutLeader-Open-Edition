#pragma once

#include "ClTaskUICommon.h"
#include "CLResizableDialog.h"
#include "taoResource.h"
#include "TaskPartInfoListDlg.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(TaskItem)

BEGIN_CUTLEADER_NAMESPACE()

class ClTaskUI_Export TaskPartManagementDlg : public CLResizableDialog
{
	DECLARE_DYNCREATE(TaskPartManagementDlg)

public:
	TaskPartManagementDlg();
	~TaskPartManagementDlg();

	enum { IDD = IDD_clTaskUI_TaskPartManagement };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnSelectOrUnSelectPartInTaskPartDlg(WPARAM wParam, LPARAM lParam);

    afx_msg void OnAddPart2TaskFromDB();
	afx_msg void OnAddPart2TaskFromDxf();
    afx_msg void OnEditTaskPartInfo();
    afx_msg void OnDeletePartFromTask();
    afx_msg void OnSelectAll();

	DECLARE_MESSAGE_MAP()

public:
    // 重新显示该对话框的数据。
    void Redisplay_TaskPartManagementDlg(TaskItemPtr pTaskItem);

    TaskItemPtr GetTaskItem() const;

    void ReDisplayNestedCount_ofEachPart();

	// 得到选中的零件。
	TaskPartListPtr GetSelectedTaskParts() const;

	TaskPartInfoListDlg* GetTaskPartInfoListDlg() { return &m_taskPartInfoListDlg; }

	// 把文件对话框中选的dxfdwg文件加入任务。
	void AddDxfdwgToTask(const std::vector<CString>& dxfdwgFullPaths, const CString& strDataCenterPath, const CString& strExpertLibItemName, const CString& strParamConfigName);

private:
    bool AreAllPartsSelected() const;
    bool IsNonePartSelected() const;

    // 更新三态复选框。
    void Update3StatusCheckBox();

private: // controls
	CBrush m_brush;

	SkinMFCButton m_ctrlAddPart2TaskFromDbButton;
	SkinMFCButton m_ctrlAddPart2TaskFromDxfButton;
	SkinMFCButton m_ctrlEditPartInfoButton;
	SkinMFCButton m_ctrlDeletePartFromTaskButton;
	CButton m_selectAllButton;

    TaskPartInfoListDlg m_taskPartInfoListDlg;

private: // data
    TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE()
