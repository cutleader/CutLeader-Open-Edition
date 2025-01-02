#include "StdAfx.h"
#include "Return2ForegroundCamBar.h"

#include "ForegroundCamConnector.h"
#include "clFrameUIResource.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(Return2ForegroundCamBar, CDialogEx)

Return2ForegroundCamBar::Return2ForegroundCamBar(CWnd* pParent /*=NULL*/)
	: CLDialog(Return2ForegroundCamBar::IDD, pParent)
{
}

Return2ForegroundCamBar::~Return2ForegroundCamBar(void)
{
}

void Return2ForegroundCamBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_Return2ForegroundCam_cancelEdit, m_btn_Return2ForegroundCam_cancelEdit);
	DDX_Control(pDX, IDC_BTN_Return2ForegroundCam_finishEdit, m_btn_Return2ForegroundCam_finishEdit);
}

BOOL Return2ForegroundCamBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btn_Return2ForegroundCam_cancelEdit.SetImage(IDB_Return2ForegroundCam_cancelEdit);
	m_btn_Return2ForegroundCam_cancelEdit.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_Return2ForegroundCam_cancelEdit));

	m_btn_Return2ForegroundCam_finishEdit.SetImage(IDB_Return2ForegroundCam_finishEdit);
	m_btn_Return2ForegroundCam_finishEdit.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_Return2ForegroundCam_finishEdit));

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

BEGIN_MESSAGE_MAP(Return2ForegroundCamBar, CDialogEx)
	ON_WM_CTLCOLOR()

    ON_BN_CLICKED(IDC_BTN_Return2ForegroundCam_cancelEdit, OnReturn2ForegroundCam_cancelEdit)
	ON_BN_CLICKED(IDC_BTN_Return2ForegroundCam_finishEdit, OnReturn2ForegroundCam_finishEdit)
END_MESSAGE_MAP()

HBRUSH Return2ForegroundCamBar::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void Return2ForegroundCamBar::OnReturn2ForegroundCam_cancelEdit()
{
	ForegroundCamConnector::GetInstance()->EditCanceled();
}

void Return2ForegroundCamBar::OnReturn2ForegroundCam_finishEdit()
{
	ForegroundCamConnector::GetInstance()->EditFinished();
}

END_CUTLEADER_NAMESPACE()
