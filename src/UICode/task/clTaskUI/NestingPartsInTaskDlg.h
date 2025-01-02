#pragma once

#include "ClTaskUICommon.h"
#include "taoResource.h"
#include "NumEdit.h"
#include "CGridListCtrlGroups.h"
#include "CLResizableDialog.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(TaskPartList)
DECLARE_CUTLEADER_CLASS(NestPartList)
DECLARE_CUTLEADER_CLASS(NestMaterialList)
DECLARE_CUTLEADER_CLASS(SheetMaterialList)
DECLARE_CUTLEADER_CLASS(NestJob)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(TaskPartsNestResult)


BEGIN_CUTLEADER_NAMESPACE()

// 用这个对话框确定要排任务中哪些零件。
class ClTaskUI_Export NestingPartsInTaskDlg : public CLResizableDialog
{
	DECLARE_DYNAMIC(NestingPartsInTaskDlg)

public:
	NestingPartsInTaskDlg(CWnd* pParent = NULL);
	virtual ~NestingPartsInTaskDlg();

	enum { IDD = IDD_clTaskUI_NestingPartsInTask };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public:
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    afx_msg void OnNestAllParts();
    afx_msg void OnNestSelectedParts();

	DECLARE_MESSAGE_MAP()

public:
	void Init(TaskItemPtr pTaskItem, TaskPartListPtr pSelectedTaskParts);

    // 得到排版零件对象。
    NestPartListPtr GetNestPartList(LONGLONG iParamConfigID) const;

private:
    // 初始化表格样式
    void InitGridStyle();

    // 检查零件是不是有和“参数配置”配套的工艺数据。
    // 注：
    //   1) 要是没有，创建一套新的工艺数据。
    //   2) 返回工艺数据ID。
    LONGLONG CheckPart(LONGLONG iParamConfigID, LONGLONG iPartID) const;

    // 填充零件表格
    void FillPartGrid_withAllPartsInTask();
    void FillPartGrid_withSelectedPartsInTask(); // 调用之前需要确保选中的零件中有未排完的零件

    // 释放零件表格中的数据
    void ReleaseData_inPartGrid();

private: // for controls
    // 零件相关
    int m_iNestedPartType;
	CGridListCtrlGroups m_partGrid;

    CBrush m_brush;

private: // data members
	// 任务对象，这里面必须要有未排完的零件。
	TaskItemPtr m_pTaskItem;

	// 选中的零件
	TaskPartListPtr m_pSelectedTaskParts;
};

END_CUTLEADER_NAMESPACE()
