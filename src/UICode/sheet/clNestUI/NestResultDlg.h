#pragma once

#include "clNestUICommon.h"
#include "taoresource.h"
#include "NumEdit.h"
#include "LineArc2DStaticControl.h"
#include "PictureEx.h"
#include "PrintWndBase.h"
#include "NestResultWatcher.h"
#include "AreaInfo.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(ReportData)
DECLARE_CUTLEADER_CLASS(NestTask)
DECLARE_CUTLEADER_CLASS(NestProcessor)
DECLARE_CUTLEADER_CLASS(NestJob)
DECLARE_CUTLEADER_CLASS(NestResult)
DECLARE_CUTLEADER_CLASS(LibraryData)

BEGIN_CUTLEADER_NAMESPACE()

// 排版结果对话框。
class NestResultDlg : public CLDialog
{
	DECLARE_DYNAMIC(NestResultDlg)

public:
	NestResultDlg(CWnd* pParent = NULL);
	~NestResultDlg(void);

	enum { IDD = IDD_clNestUI_NestResult };

protected: // inherit from the base class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	virtual void OnCancel() { return; }

public: // message mapping.
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg BOOL OnMouseWheel(UINT nFlags, short iDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnSelchangedSheet(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSaveRemnant();

	afx_msg void OnAllResult();
	afx_msg void OnStopNest();

	afx_msg void OnAccept();
	afx_msg void OnQuit();

	DECLARE_MESSAGE_MAP()

public:
	NestJobPtr GetNestJob() const { return m_pNestJob; }
	void Init(NestTaskPtr pNestTask, NestProcessorPtr pNestProcessor, const AreaInfo& areaInfo, LibraryDataPtr pLibraryData);

private:
	void InitListStyle();

	void DisplayNestResult();

	// 初始化预览数据。
	void InitPreviewData(int iRow);

	// preview sheet
	void PreviewSheet();

	// 释放表格中资源。
	void ReleaseData_in_sheetGrid();

private: // for controls.
	// for "Usage of Material" group.
	CNumEdit m_ctrlTotalMatNum;
	CNumEdit m_ctrlUsedMatNum;
	CString m_strUtilization;

	// for "Elapsed Time(s)" group.
	CNumEdit m_ctrlNestTime;

	// for "Nested Part Statistics" group.
	CNumEdit m_ctrlSubmitPartNum;
	CNumEdit m_ctrlNestedPartNum;
	SkinGridListCtrlGroups m_nestPartGrid;

	// for "Nest Result files" group.
	SkinGridListCtrlGroups m_sheetGrid;
	SkinMFCButton m_ctrlSaveRemantMat;

	// for Nesting Progress.
	CString m_strStatus;
	CString m_strRetRstNum;
	CPictureEx m_ctrlTaskProgress;
	SkinMFCButton m_btnStop;
	SkinMFCButton m_btnAllResult;

	// 报表模板。
	CComboBox m_ctrlRptTpl;

	// wnd for preview.
	LineArc2DStaticControl m_previewWnd;
	CString m_strPartRegionRect;

	SkinMFCButton m_btnAccept;
	SkinMFCButton m_btnQuit;

	// the timer.
	int m_timer;

private: // data.
	NestTaskPtr m_pNestTask;
	NestProcessorPtr m_pNestProcessor;

	// use this to preview the sheet.
	LineArc2DListPtr m_pGeom2DList_of_part; // 零件
	LineArc2DListPtr m_pGeom2DList_of_mat; // 板材外框
	LineArc2DListPtr m_pDashGeom2DList_of_mat; // 板材无效区域

	BOOL m_bDlgInited;

	// 当前显示的排版结果。
	NestJobPtr m_pNestJob;

	// the count of the returned nesting result.
	unsigned int m_iRetRstNum;

	// the nesting watcher.
	NestResultWatcher m_nestRstWatcher;

	// the nesting result.
	NestResultPtr m_pNestResult;

	// 缓存零件面积。
	AreaInfo m_areaInfo;

	// 预先加载好的要用到的库信息。
	LibraryDataPtr m_pLibraryData;
};

END_CUTLEADER_NAMESPACE()
