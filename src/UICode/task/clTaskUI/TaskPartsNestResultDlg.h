#pragma once

#include "ClTaskUICommon.h"
#include "taoResource.h"
#include "NumEdit.h"
#include "CGridListCtrlGroups.h"
#include "PictureEx.h"
#include "NestResultWatcher.h"
#include "LineArc2DStaticControl.h"
#include "SkinManager.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(NestTask)
DECLARE_CUTLEADER_CLASS(NestProcessor)
DECLARE_CUTLEADER_CLASS(LibraryData)
DECLARE_CUTLEADER_CLASS(NestResult)
DECLARE_CUTLEADER_CLASS(NestJob)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(SheetMaterialList)
DECLARE_CUTLEADER_CLASS(TaskPartsNestResult)


BEGIN_CUTLEADER_NAMESPACE()

// 排版结果对话框
class ClTaskUI_Export TaskPartsNestResultDlg : public CLDialog
{
	DECLARE_DYNAMIC(TaskPartsNestResultDlg)

public:
	TaskPartsNestResultDlg(CWnd* pParent = NULL);
	virtual ~TaskPartsNestResultDlg();

	enum { IDD = IDD_clTaskUI_TaskPartsNestResult };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);

    afx_msg BOOL OnMouseWheel(UINT nFlags, short iDelta, CPoint pt);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);

    afx_msg void OnSelchangedExistingSheet(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchangedNewSheet(NMHDR* pNMHDR, LRESULT* pResult);

    afx_msg void OnAllResult();
    afx_msg void OnStopNest();

	afx_msg void OnAccept();
	afx_msg void OnQuit();

	DECLARE_MESSAGE_MAP()

public:
    TaskPartsNestResultPtr GetTaskPartsNestResult() const { return m_pTaskPartsNestResult; }
	void Init(TaskItemPtr pTaskItem, NestTaskPtr pNestTask, NestProcessorPtr pNestProcessor, LibraryDataPtr pLibraryData, SheetMaterialListPtr pSheetMaterials);

private:
    // 初始化表格样式
    void InitGridStyle();

	void DisplayNestResult();

	// 初始化预览数据。
	void InitPreviewData(int iRow, bool bFromExistingSheetsGrid);

	// preview sheet
	void PreviewSheet();

	// 释放表格中资源。
	void ReleaseData_in_grid();

private: // for controls
	CGridListCtrlGroups m_nestedPartsGrid;
	CGridListCtrlGroups m_newSheetsGrid;
	CGridListCtrlGroups m_existingSheetsGrid;
    CString m_strStatus;
    CString m_strRetRstNum;
    CNumEdit m_ctrlNestTime;
    CPictureEx m_ctrlTaskProgress;

	// wnd for preview.
	LineArc2DStaticControl m_previewWnd;

	SkinMFCButton m_btnStop;
	SkinMFCButton m_btnAllResult;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

    CBrush m_brush;

	// the timer.
	int m_timer;

private: // data members
    // 外面送进来的数据
    TaskItemPtr m_pTaskItem;
	NestTaskPtr m_pNestTask;
	NestProcessorPtr m_pNestProcessor;
	LibraryDataPtr m_pLibraryData;
	SheetMaterialListPtr m_pSheetMaterials;

	// the nesting watcher.
	NestResultWatcher m_nestRstWatcher;

	// 来自排版引擎的排版结果
	NestResultPtr m_pNestResult;

	// the count of the returned nesting result.
	unsigned int m_iRetRstNum;

	// 当前显示的排版结果。
    TaskPartsNestResultPtr m_pTaskPartsNestResult;

	// 用于预览板材
	LineArc2DListPtr m_pLineArc2Ds_of_part; // 零件
	LineArc2DListPtr m_pLineArc2Ds_of_mat; // 板材外框
	LineArc2DListPtr m_pDashLineArc2Ds_of_mat; // 板材无效区域

    BOOL m_bDisableGridSelectChangeEvent;
};

END_CUTLEADER_NAMESPACE()
