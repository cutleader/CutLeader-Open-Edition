#include "stdafx.h"
#include "SubToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(SubToolBar, CLDialog)

SubToolBar::SubToolBar()
{
}

SubToolBar::SubToolBar(UINT nIDTemplate, CWnd* pParentWnd)
	: CLDialog(nIDTemplate, pParentWnd)
{
}

SubToolBar::~SubToolBar()
{
}

void SubToolBar::PostNcDestroy()
{
	CLDialog::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(SubToolBar, CLDialog)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()

HBRUSH SubToolBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void SubToolBar::OnSize(UINT nType, int cx, int cy)
{
	CLDialog::OnSize(nType, cx, cy);
}

END_CUTLEADER_NAMESPACE()
