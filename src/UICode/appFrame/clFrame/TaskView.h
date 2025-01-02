#pragma once

#include "clFrameCommon.h"
#include "PrintWndBase.h"
#include "ObserverBase.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(ReportData)


BEGIN_CUTLEADER_NAMESPACE()

// ������ͼ
// ע��
//  1) ������ͼ����ֱ�ӽ��ܹ����������������Ҫͨ�����ⷢ��Ϣ�ķ�ʽ����������ͼ��
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

	// �������ֻ���ڵ�ǰ��ǩҳ�����л�ʱ�ű����ã�Ŀǰ��������Ӧ�İ��ȥ��ʼ�������ͼ��
	// ע��
	// 1) Ŀǰ�û������ȥ�����ǩҳʱ��������������д���������Щ�ط��������ô���ɾ����ǩҳʱ�ͻ��������¼���ԭ��������¼����ܻ��ظ����Ͷ�Σ�����û��Ҫ�ķ�������
	//    ��������£����������ط�����������������е��߼���
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
    // ���ɱ���
    BOOL GenerateTaskRpt();

    // ���ݱ�ǩҳ�����õ��������ͼ����
    CView* GetSheetSubViewByTabIndex(int iTabIndex) const;

private:
    // the report data.
    ReportDataPtr m_pReportData;

	// ����������������Ƿ����ǩҳ�����¼���Ĭ���Ǵ���ģ��еĳ��ϻ᲻������Ϊ�˱����ظ�������¼���
	bool m_bDisableTabActiveEvent;
};

END_CUTLEADER_NAMESPACE();
