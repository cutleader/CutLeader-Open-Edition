#include "StdAfx.h"
#include "WorkingView.h"

#include <..\src\mfc\afximpl.h>
#include "PreviewViewEx.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(WorkingView, CView)

WorkingView::WorkingView(void)
{
	m_pPrintWnd = NULL;
	m_pCurrentFrame = NULL;
	m_bDestroyParentFrm = TRUE;
}

WorkingView::~WorkingView(void)
{
	m_pPrintWnd = NULL;
	m_pCurrentFrame = NULL;
}

void WorkingView::OnDraw(CDC* pDC)
{
}

BOOL WorkingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	if (m_pPrintWnd)
	{
		m_pPrintWnd->PreparePrinting(pInfo);
		return DoPreparePrinting(pInfo);
	}

	return FALSE;
}

void WorkingView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_pPrintWnd)
	{
		m_pPrintWnd->BeginPrinting(pDC, pInfo);
	}
}

void WorkingView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_pPrintWnd)
	{
		m_pPrintWnd->PrepareDC(pDC, pInfo);
	}
}

void WorkingView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_pPrintWnd)
	{
		m_pPrintWnd->Print(pDC, pInfo);
	}
}

void WorkingView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_pPrintWnd)
	{
		m_pPrintWnd->EndPrinting(pDC, pInfo);
	}
}

void WorkingView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView)
{
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);

	// show the original frame
	m_pCurrentFrame->ShowWindow(SW_SHOW);

	// restore main frame pointer
	AfxGetApp()->m_pMainWnd = m_pCurrentFrame;
	m_pPrintWnd->SetWndFocus();

	// 是否销毁父框架。
	//if (m_bDestroyParentFrm) // alway need to destroy parent frame, if enable this code, the preview window will not disappear after click X button.
		GetParentFrame()->DestroyWindow();
}

BEGIN_MESSAGE_MAP(WorkingView, CView)

END_MESSAGE_MAP()

void WorkingView::OnPrintPreview()
{
	// In derived classes, implement special window handling here
	// Be sure to Unhook Frame Window close if hooked.

	// must not create this on the frame.  Must outlive this function
	CPrintPreviewState* pState = new CPrintPreviewState;

	// DoPrintPreview's return value does not necessarily indicate that
	// Print preview succeeded or failed, but rather what actions are necessary
	// at this point.  If DoPrintPreview returns TRUE, it means that
	// OnEndPrintPreview will be (or has already been) called and the
	// pState structure will be/has been deleted.
	// If DoPrintPreview returns FALSE, it means that OnEndPrintPreview
	// WILL NOT be called and that cleanup, including deleting pState
	// must be done here.

	if (!DoPrintPreview(AFX_IDD_PREVIEW_TOOLBAR, this, RUNTIME_CLASS(PreviewViewEx), pState))
	{
		// In derived classes, reverse special window handling here for Preview failure case

		TRACE0("Error: DoPrintPreview failed.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		delete pState;      // preview failed to initialize, delete State now
	}

	// the preview view should be the active view, set the pointer for it
	// we need to do this so that the ID_FILE_PRINT command will goto the dialog
	// and not any global view.
	CFrameWnd *pWnd = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	PreviewViewEx *pPreviewViewEx = (PreviewViewEx*)pWnd->GetActiveView();
	pPreviewViewEx->SetPrintWnd(m_pPrintWnd);
}

void WorkingView::OnFilePrint()
{
	// get default print info
	CPrintInfo printInfo;
	ASSERT(printInfo.m_pPD != NULL);    // must be set

	if (LOWORD(GetCurrentMessage()->wParam) == ID_FILE_PRINT_DIRECT)
	{
		CCommandLineInfo* pCmdInfo = AfxGetApp()->m_pCmdInfo;

		if (pCmdInfo != NULL)
		{
			if (pCmdInfo->m_nShellCommand == CCommandLineInfo::FilePrintTo)
			{
				printInfo.m_pPD->m_pd.hDC = ::CreateDC(pCmdInfo->m_strDriverName,
													   pCmdInfo->m_strPrinterName, pCmdInfo->m_strPortName, NULL);
				if (printInfo.m_pPD->m_pd.hDC == NULL)
				{
					AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
					return;
				}
			}
		}

		printInfo.m_bDirect = TRUE;
	}

	if (OnPreparePrinting(&printInfo))
	{
		// hDC must be set (did you remember to call DoPreparePrinting?)
		ASSERT(printInfo.m_pPD->m_pd.hDC != NULL);

		// gather file to print to if print-to-file selected
		CString strOutput;
		if (printInfo.m_pPD->m_pd.Flags & PD_PRINTTOFILE && !printInfo.m_bDocObject)
		{
			// construct CFileDialog for browsing
			CString strDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULTEXT));
			CString strPrintDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULT));
			CString strFilter(MAKEINTRESOURCE(AFX_IDS_PRINTFILTER));
			CString strCaption(MAKEINTRESOURCE(AFX_IDS_PRINTCAPTION));
			CFileDialog dlg(FALSE, strDef, strPrintDef, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);
			dlg.m_ofn.lpstrTitle = strCaption;

			if (dlg.DoModal() != IDOK)
				return;

			// set output device to resulting path name
			strOutput = dlg.GetPathName();
		}

		// set up document info and start the document printing process
		CString strTitle;
		CDocument* pDoc = GetDocument();
		if (pDoc != NULL)
			strTitle = pDoc->GetTitle();
		else
			strTitle = m_pPrintWnd->GetWndText();
		if (strTitle.GetLength() > 31)
			strTitle.ReleaseBuffer(31);
		DOCINFO docInfo;
		memset(&docInfo, 0, sizeof(DOCINFO));
		docInfo.cbSize = sizeof(DOCINFO);
		docInfo.lpszDocName = strTitle;
		CString strPortName;
		int nFormatID;
		if (strOutput.IsEmpty())
		{
			docInfo.lpszOutput = NULL;
			strPortName = printInfo.m_pPD->GetPortName();
			nFormatID = AFX_IDS_PRINTONPORT;
		}
		else
		{
			docInfo.lpszOutput = strOutput;
			AfxGetFileTitle(strOutput, strPortName.GetBuffer(_MAX_PATH), _MAX_PATH);
			nFormatID = AFX_IDS_PRINTTOFILE;
		}

		// setup the printing DC
		CDC dcPrint;
		if (!printInfo.m_bDocObject)
		{
			dcPrint.Attach(printInfo.m_pPD->m_pd.hDC);  // attach printer dc
			dcPrint.m_bPrinting = TRUE;
		}
		OnBeginPrinting(&dcPrint, &printInfo);

		//if (!printInfo.m_bDocObject)
		//	dcPrint.SetAbortProc(AbortProc);

		// disable main window while printing & init printing status dialog
		AfxGetMainWnd()->EnableWindow(FALSE);
		//CPrintingDialog dlgPrintStatus(this);

		//CString strTemp;
		//dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_DOCNAME, strTitle);
		//dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PRINTERNAME,
		//	printInfo.m_pPD->GetDeviceName());
		//AfxFormatString1(strTemp, nFormatID, strPortName);
		//dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PORTNAME, strTemp);
		//dlgPrintStatus.ShowWindow(SW_SHOW);
		//dlgPrintStatus.UpdateWindow();

		// start document printing process
		if (!printInfo.m_bDocObject && dcPrint.StartDoc(&docInfo) == SP_ERROR)
		{
			// enable main window before proceeding
			AfxGetMainWnd()->EnableWindow(TRUE);

			// cleanup and show error message
			OnEndPrinting(&dcPrint, &printInfo);
			//dlgPrintStatus.DestroyWindow();
			dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
			AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
			return;
		}

		// Guarantee values are in the valid range
		UINT nEndPage = printInfo.GetToPage();
		UINT nStartPage = printInfo.GetFromPage();

		if (nEndPage < printInfo.GetMinPage())
			nEndPage = printInfo.GetMinPage();
		if (nEndPage > printInfo.GetMaxPage())
			nEndPage = printInfo.GetMaxPage();

		if (nStartPage < printInfo.GetMinPage())
			nStartPage = printInfo.GetMinPage();
		if (nStartPage > printInfo.GetMaxPage())
			nStartPage = printInfo.GetMaxPage();

		int nStep = (nEndPage >= nStartPage) ? 1 : -1;
		nEndPage = (nEndPage == 0xffff) ? 0xffff : nEndPage + nStep;

		//VERIFY(strTemp.LoadString(AFX_IDS_PRINTPAGENUM));

		// If it's a doc object, we don't loop page-by-page
		// because doc objects don't support that kind of levity.

		BOOL bError = FALSE;
		if (printInfo.m_bDocObject)
		{
			OnPrepareDC(&dcPrint, &printInfo);
			OnPrint(&dcPrint, &printInfo);
		}
		else
		{
			// begin page printing loop
			for (printInfo.m_nCurPage = nStartPage; printInfo.m_nCurPage != nEndPage; printInfo.m_nCurPage += nStep)
			{
				OnPrepareDC(&dcPrint, &printInfo);

				// check for end of print
				if (!printInfo.m_bContinuePrinting)
					break;

				// write current page
				//TCHAR szBuf[80];
				//wsprintf(szBuf, strTemp, printInfo.m_nCurPage);
				//dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PAGENUM, szBuf);

				// set up drawing rect to entire page (in logical coordinates)
				printInfo.m_rectDraw.SetRect(0, 0, dcPrint.GetDeviceCaps(HORZRES), dcPrint.GetDeviceCaps(VERTRES));
				dcPrint.DPtoLP(&printInfo.m_rectDraw);

				// attempt to start the current page
				if (dcPrint.StartPage() < 0)
				{
					bError = TRUE;
					break;
				}

				// must call OnPrepareDC on newer versions of Windows because
				// StartPage now resets the device attributes.
				OnPrepareDC(&dcPrint, &printInfo);

				ASSERT(printInfo.m_bContinuePrinting);

				// page successfully started, so now render the page
				OnPrint(&dcPrint, &printInfo);

				if (dcPrint.EndPage() < 0)// || !AbortProc(dcPrint.m_hDC, 0))
				{
					bError = TRUE;
					break;
				}
			}
		}

		// cleanup document printing process
		if (!printInfo.m_bDocObject)
		{
			if (!bError)
				dcPrint.EndDoc();
			else
				dcPrint.AbortDoc();
		}

		AfxGetMainWnd()->EnableWindow();    // enable main window

		OnEndPrinting(&dcPrint, &printInfo);    // clean up after printing
		//dlgPrintStatus.DestroyWindow();

		dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
	}
}

END_CUTLEADER_NAMESPACE()
