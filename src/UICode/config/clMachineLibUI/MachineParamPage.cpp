#include "stdafx.h"
#include "MachineParamPage.h"

#include "baseConst.h"
#include "PathHelper.h"
#include "CryptoUtil.h"
#include "RandomGen.h"
#include "versionInfo.h"
#include "clBaseDataResource.h"
#include "DataBaseManager.h"
#include "ClFolderHelper.h"

#include "MachineParam.h"
#include "OverTravel.h"
#include "MachineTable.h"
#include "DriverData.h"
#include "clMachineLibResource.h"
#include "MachineItemLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const size_t MAX_TIP_TEXT_LENGTH = 1024;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(MachineParamPage, CResizableDialog)

MachineParamPage::MachineParamPage() : CLResizableDialog(MachineParamPage::IDD)
{
}

MachineParamPage::~MachineParamPage()
{
}

void MachineParamPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	// machine table
	DDX_Control(pDX, IDC_EDIT_TABLE_X, m_dTableX);
	DDX_Control(pDX, IDC_EDIT_TABLE_Y, m_dTableY);

	// OverTravel
	DDX_Control(pDX, IDC_EDIT_MIN_OT_X, m_dMinOTX);
	DDX_Control(pDX, IDC_EDIT_MIN_OT_Y, m_dMinOTY);
	DDX_Control(pDX, IDC_EDIT_MAX_OT_X, m_dMaxOTX);
	DDX_Control(pDX, IDC_EDIT_MAX_OT_Y, m_dMaxOTY);

	DDX_Text(pDX, IDC_EDIT_DRI, m_driName);
	DDX_Text(pDX, IDC_EDIT_NC_POSTFIX, m_strNCPostFix);

	DDX_Control(pDX, IDC_BTN_SEL_DRI, m_btnSel);
}

BEGIN_MESSAGE_MAP(MachineParamPage, CResizableDialog)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()

	// machine table
	ON_EN_KILLFOCUS(IDC_EDIT_TABLE_X, OnKillTableX)
	ON_EN_KILLFOCUS(IDC_EDIT_TABLE_Y, OnKillTableY)

	// OverTravel
	ON_EN_KILLFOCUS(IDC_EDIT_MIN_OT_X, OnKillMinOTX)
	ON_EN_KILLFOCUS(IDC_EDIT_MIN_OT_Y, OnKillMinOTY)
	ON_EN_KILLFOCUS(IDC_EDIT_MAX_OT_X, OnKillMaxOTX)
	ON_EN_KILLFOCUS(IDC_EDIT_MAX_OT_Y, OnKillMaxOTY)

	ON_BN_CLICKED(IDC_BTN_SEL_DRI, OnSelectDriver)
	ON_EN_KILLFOCUS(IDC_EDIT_NC_POSTFIX, OnKillNCPostfix)
END_MESSAGE_MAP()


BOOL MachineParamPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	LayoutCtrl();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	
	SetWindowTheme( GetDlgItem( IDC_STATIC_TABLE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_OVERTRAVEL )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_NC )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);
	return TRUE;
}

HBRUSH MachineParamPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void MachineParamPage::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
}

void MachineParamPage::LayoutCtrl()
{
	// machine table
	AddAnchor(IDC_STATIC_TABLE, TOP_LEFT);

	AddAnchor(IDC_TABLEX, TOP_LEFT);
	AddAnchor(IDC_EDIT_TABLE_X, TOP_LEFT);
	AddAnchor(IDC_TABLEY, TOP_LEFT);
	AddAnchor(IDC_EDIT_TABLE_Y, TOP_LEFT);

	// OverTravel
	AddAnchor(IDC_STATIC_OVERTRAVEL, TOP_LEFT);
	AddAnchor(IDC_MINOTX, TOP_LEFT);
	AddAnchor(IDC_EDIT_MIN_OT_X, TOP_LEFT);
	AddAnchor(IDC_EDIT_MIN_OTY, TOP_LEFT);
	AddAnchor(IDC_EDIT_MIN_OT_Y, TOP_LEFT);
	AddAnchor(IDC_MAXOTX, TOP_LEFT);
	AddAnchor(IDC_EDIT_MAX_OT_X, TOP_LEFT);
	AddAnchor(IDC_EDIT_MAX_OTY, TOP_LEFT);
	AddAnchor(IDC_EDIT_MAX_OT_Y, TOP_LEFT);

	// NC
	AddAnchor(IDC_STATIC_NC, TOP_LEFT);
	AddAnchor(IDC_STATIC_DRI, TOP_LEFT);
	AddAnchor(IDC_EDIT_DRI, TOP_LEFT);
	AddAnchor(IDC_BTN_SEL_DRI, TOP_LEFT);
	AddAnchor(IDC_STATIC_NC_POSTFIX, TOP_LEFT);
	AddAnchor(IDC_EDIT_NC_POSTFIX, TOP_LEFT);
}

void MachineParamPage::DisplayMacParam(MachineParamPtr pMacParam)
{
	m_pMacParam = pMacParam;

	// machine table
	m_dTableX.Init(pMacParam->GetMacTable()->GetTableSizeX(), DIGITAL_NUM, FALSE);
	m_dTableY.Init(pMacParam->GetMacTable()->GetTableSizeY(), DIGITAL_NUM, FALSE);

	// over travel
	OverTravelPtr pOT = pMacParam->GetOverTravel();
	m_dMinOTX.Init(pOT->GetMinOTX(), DIGITAL_NUM);
	m_dMinOTY.Init(pOT->GetMinOTY(), DIGITAL_NUM);
	m_dMaxOTX.Init(pOT->GetMaxOTX(), DIGITAL_NUM, FALSE);
	m_dMaxOTY.Init(pOT->GetMaxOTY(), DIGITAL_NUM, FALSE);

	// driver data.
	DriverDataPtr pDriverData = pMacParam->GetDriverData();
	m_driName = pDriverData->GetDriName();

	// postfix of NC code.
	m_strNCPostFix = pMacParam->GetNCPostFix();

	UpdateData(FALSE);
}

MachineParamPtr MachineParamPage::GetMacParam()
{
	return m_pMacParam;
}

void MachineParamPage::OnKillTableX()
{
	UpdateData(TRUE);

	if (m_pMacParam)
	{
		MachineTablePtr pMacTable = m_pMacParam->GetMacTable();
		if (pMacTable->GetTableSizeX() != m_dTableX.Get_Number())
		{
			pMacTable->SetTableSizeX(m_dTableX.Get_Number());
			m_pMacParam->SetModified(TRUE);
		}
	}
}

void MachineParamPage::OnKillTableY()
{
	UpdateData(TRUE);

	MachineTablePtr pMacTable = m_pMacParam->GetMacTable();
	if (pMacTable->GetTableSizeY() != m_dTableY.Get_Number())
	{
		pMacTable->SetTableSizeY(m_dTableY.Get_Number());
		m_pMacParam->SetModified(TRUE);
	}
}

void MachineParamPage::OnKillMinOTX()
{
	UpdateData(TRUE);

	OverTravelPtr pOT = m_pMacParam->GetOverTravel();
	if (pOT->GetMinOTX() != m_dMinOTX.Get_Number())
	{
		pOT->SetMinOTX(m_dMinOTX.Get_Number());
		m_pMacParam->SetModified(TRUE);
	}
}

void MachineParamPage::OnKillMinOTY()
{
	UpdateData(TRUE);

	OverTravelPtr pOT = m_pMacParam->GetOverTravel();
	if (pOT->GetMinOTY() != m_dMinOTY.Get_Number())
	{
		pOT->SetMinOTY(m_dMinOTY.Get_Number());
		m_pMacParam->SetModified(TRUE);
	}
}

void MachineParamPage::OnKillMaxOTX()
{
	UpdateData(TRUE);

	OverTravelPtr pOT = m_pMacParam->GetOverTravel();
	if (pOT->GetMaxOTX() != m_dMaxOTX.Get_Number())
	{
		pOT->SetMaxOTX(m_dMaxOTX.Get_Number());
		m_pMacParam->SetModified(TRUE);
	}
}

void MachineParamPage::OnKillMaxOTY()
{
	UpdateData(TRUE);

	OverTravelPtr pOT = m_pMacParam->GetOverTravel();
	if (pOT->GetMaxOTY() != m_dMaxOTY.Get_Number())
	{
		pOT->SetMaxOTY(m_dMaxOTY.Get_Number());
		m_pMacParam->SetModified(TRUE);
	}
}

void MachineParamPage::OnKillNCPostfix()
{
	UpdateData(TRUE);

	if (m_pMacParam->GetNCPostFix() != m_strNCPostFix)
	{
		m_pMacParam->SetNCPostFix(m_strNCPostFix);
		m_pMacParam->SetModified(TRUE);
	}
}

void MachineParamPage::OnSelectDriver()
{
	CString strFilter;
	strFilter = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DRI_FILTER);
	CFileDialog openFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, strFilter);
	if (openFileDlg.DoModal() == IDOK)
	{
		CString strFilePath = openFileDlg.GetPathName();

		// read binary data from .CLDri file.
		HANDLE hFile = CreateFile(strFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		void* pBuf = malloc(MAXLENGTH_STR_1M*4);
		DWORD dwBytesRead = 0;
		::ReadFile(hFile, pBuf, MAXLENGTH_STR_1M*4, &dwBytesRead, NULL);

		// build DriverData object.
		DriverDataPtr pDriverData(new DriverData);
		pDriverData->SetDriName(PathHelper::GetFileName(strFilePath));
		pDriverData->SetDriData(pBuf);
		pDriverData->SetDriSize(dwBytesRead);

		// check the driver version.
		CString strDriVer = MachineItemLoader::GetDriverVersion(pDriverData);
		if (strDriVer != DRIVER_VERSION)
		{
			CString strProductName, str;
			strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DRI_VER_INCORRECT);
			MessageBox(str, strProductName, MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			// update the driver data of the machine.
			m_pMacParam->SetDriverData(pDriverData);
			m_pMacParam->SetModified(TRUE);

			m_driName = pDriverData->GetDriName();
			UpdateData(FALSE);
		}

		// close file.
		::CloseHandle(hFile);
	}
}

END_CUTLEADER_NAMESPACE()
