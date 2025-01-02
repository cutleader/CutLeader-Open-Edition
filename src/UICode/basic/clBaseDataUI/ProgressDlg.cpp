#include "StdAfx.h"
#include "ProgressDlg.h"

#include "baseConst.h"
#include "TaskEx.h"
#include "ProgressData.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ProgressDlg, CDialogEx)

BEGIN_MESSAGE_MAP(ProgressDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_MESSAGE(WM_UPDATE_PROGRESS, OnUpdateProgress)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnCancel)
END_MESSAGE_MAP()

ProgressDlg::ProgressDlg() : CDialogEx(ProgressDlg::IDD)
{
}

ProgressDlg::~ProgressDlg(void)
{
}

void ProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TASK_PROGRESS, m_ctrlTaskProgress);
	DDX_Control(pDX, IDC_EDIT_SUBTASK, m_ctrlSubTask);
	DDX_Control( pDX, IDC_BTN_CANCEL, m_btnCancal );
}

BOOL ProgressDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// set the title.
	SetWindowText(m_strTitle);

	// init the progress bar.
	m_ctrlTaskProgress.SetRange(0, 100);
	m_ctrlTaskProgress.SetPos(0);

	// 设置可显示字符数量。
	m_ctrlSubTask.SetLimitText(-1);

	// set the window handle to the task and start it.
	m_pTask->SetProgressWnd(m_hWnd);
	m_pTask->Start();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_TASK_STATUS )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

HBRUSH ProgressDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

LRESULT ProgressDlg::OnUpdateProgress(WPARAM wParam, LPARAM lParam)
{
	// wait for read progress data.
	WaitForSingleObject(m_pTask->GetProgressData()->GetProgressDataMtx(), INFINITE);

	// update progress bar.
	m_ctrlTaskProgress.SetPos((int)m_pTask->GetProgressData()->GetTaskPercent());

	// update the text box.
	int iLen = m_ctrlSubTask.GetWindowTextLength(); 
	m_ctrlSubTask.SetSel(iLen, iLen); 
	m_ctrlSubTask.ReplaceSel(m_pTask->GetProgressData()->GetSubTask() + _T("\n"));

	// whether the task is finished.
	if (m_pTask->GetProgressData()->IsFinished())
	{
		EndDialog(0);
	}

	// release the mutex.
	ReleaseMutex(m_pTask->GetProgressData()->GetProgressDataMtx());

	return true;
}

void ProgressDlg::OnCancel()
{
	// wait for read progress data.
	WaitForSingleObject(m_pTask->GetProgressData()->GetProgressDataMtx(), INFINITE);

	m_pTask->GetProgressData()->IsCanceled(TRUE);

	// release the mutex.
	ReleaseMutex(m_pTask->GetProgressData()->GetProgressDataMtx());
}

void ProgressDlg::Init(CString strTitle, TaskExPtr pTask)
{
	m_strTitle = strTitle;
	m_pTask = pTask;
}

END_CUTLEADER_NAMESPACE()
