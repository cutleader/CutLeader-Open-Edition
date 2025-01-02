#pragma once

#include "clFrameCommon.h"
#include "PrintWndBase.h"
#include "ObserverBase.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(ReportData)


BEGIN_CUTLEADER_NAMESPACE()

// 任务视图
// 注：
//  1) 任务视图不能直接接受工具条的命令，所以需要通过额外发消息的方式告诉任务视图。
class CLFRAME_Export TaskView : public CTabView, public PrintWndBase, public ObserverBase
{
protected:
    DECLARE_DYNCREATE(TaskView)

	TaskView();

public:
    virtual ~TaskView();

public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// 这个函数只有在当前标签页发生切换时才被调用，目前用来用相应的板材去初始化板材视图。
	// 注：
	// 1) 目前用户用鼠标去激活标签页时会用这个函数进行处理。其他有些地方，比如用代码删除标签页时就会忽略这个事件，原因是这个事件可能会重复发送多次，导致没必要的反复处理，
	//    这种情况下，会在其他地方处理类似这个函数中的逻辑。
    virtual void OnActivateView(CView* pView);

    virtual void PreparePrinting(CPrintInfo* pInfo);
    virtual void Print(CDC* pDC, CPrintInfo* pInfo);
    virtual CString GetWndText();
    virtual void SetWndFocus();

public: // implement IObserver interface.
    virtual void Update(int iEventID, const CComVariant& varEventContent);

public:
    afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    afx_msg LRESULT OnOpenLastSheetInTask(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDeleteSheetSubView(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnActiveSheetSubView(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnPlacePartOnCurrentSheetSubView(WPARAM wParam, LPARAM lParam);

    afx_msg LRESULT OnPreviewTaskReport(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnPrintTaskReport(WPARAM wParam, LPARAM lParam);

    afx_msg LRESULT OnExecuteUndoCommand(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnExecuteRedoCommand(WPARAM wParam, LPARAM lParam);

    afx_msg LRESULT OnNestTaskParts(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnReDisplayNestedCountOfEachPart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRedrawSheetThumbnailOnSheetManagementPanel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReDisplayTaskSheets(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPartAddedOnSheet(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddPattern2CurrentTask(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateLoopToolPanel(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
    void InitTaskView();
    void InitTaskView_4SaveAsTask();

	TaskItemPtr GetTaskItem() const;

private:
    // 生成报表。
    BOOL GenerateTaskRpt();

    // 根据标签页索引得到板材子视图对象。
    CView* GetSheetSubViewByTabIndex(int iTabIndex) const;

private:
    // the report data.
    ReportDataPtr m_pReportData;

	// 这个开关用来控制是否处理标签页激活事件，默认是处理的，有的场合会不处理是为了避免重复处理该事件。
	bool m_bDisableTabActiveEvent;
};

END_CUTLEADER_NAMESPACE();
