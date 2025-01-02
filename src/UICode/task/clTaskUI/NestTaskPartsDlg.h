#pragma once

#include "ClTaskUICommon.h"
#include "taoResource.h"
#include "NumEdit.h"
#include "CGridListCtrlGroups.h"
#include "SkinManager.h"


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

// 用这个对话框排任务中未排完的零件。
class ClTaskUI_Export NestTaskPartsDlg : public CLDialog
{
	DECLARE_DYNAMIC(NestTaskPartsDlg)

public:
	NestTaskPartsDlg(CWnd* pParent = NULL);
	virtual ~NestTaskPartsDlg();

	enum { IDD = IDD_clTaskUI_NestTaskParts };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public:
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    afx_msg void OnNestAllParts();
    afx_msg void OnNestSelectedParts();
	afx_msg void OnBatchSetting();

    afx_msg void OnNestOnCurrentSheet();
    afx_msg void OnNestOnNewMaterial();
    afx_msg void OnAddMat();
    afx_msg void OnDeleteMat();

    afx_msg void OnEditNestParam();
	afx_msg void OnStartNesting();
    afx_msg void OnCancelNesting();

	DECLARE_MESSAGE_MAP()

public:
	void Init(TaskItemPtr pTaskItem, TaskPartListPtr pSelectedTaskParts, SheetPtr pActiveSheet);
    TaskPartsNestResultPtr GetTaskPartsNestResult() const { return m_pTaskPartsNestResult; }

private:
    // 初始化表格样式
    void InitGridStyle();


    /************************************************************************/
    // 零件相关
    
	// 得到排版零件对象。
	NestPartListPtr GetNestPartList(LONGLONG iParamConfigID) const;

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
    /************************************************************************/


    /************************************************************************/
    // 材料相关

	// 得到材料。
	NestMaterialListPtr GetNestMatList(SheetMaterialListPtr pSheetMaterials) const;

    // 是否添加了这个零件
    BOOL MatAdded(LONGLONG iMatSizeID);

    // 向列表中添加一行。
    void AddNestMaterial(const IMaterialSize* pMaterialSize);

    // 释放材料表格中的数据
    void ReleaseData_inSheetsInTaskGrid();
    void ReleaseData_inNewMaterialGrid();
    /************************************************************************/


	void DisplayInvalidParts(const std::vector<CString>& openBoundaryPartNames);

private: // for controls
    // 零件相关
    int m_iNestedPartType;
	CGridListCtrlGroups m_partGrid;

    // 材料相关
    int m_iUseMaterialType;
	CGridListCtrlGroups m_grid_sheetsInTask;
	CGridListCtrlGroups m_grid_newMaterial;

    SkinMFCButton m_btnBatchSetPart;
    SkinMFCButton m_btnAddMaterial;
    SkinMFCButton m_btnDeleteMaterial;
    SkinMFCButton m_btnNestParam;
    SkinMFCButton m_btnStartNest;
    SkinMFCButton m_btnCancel;

    CBrush m_brush;

private: // data members
	// 任务对象，这里面必须要有未排完的零件。
	TaskItemPtr m_pTaskItem;

	// 选中的零件
	TaskPartListPtr m_pSelectedTaskParts;

	// 当前的板材
	SheetPtr m_pActiveSheet;

    // 排版结果
    TaskPartsNestResultPtr m_pTaskPartsNestResult;
};

END_CUTLEADER_NAMESPACE()
