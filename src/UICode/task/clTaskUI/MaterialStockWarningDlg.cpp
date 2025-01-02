#include "StdAfx.h"
#include "MaterialStockWarningDlg.h"

#include "CGridColumnTraitText.h"
#include "baseConst.h"
#include "ClUtilityResource.h"
#include "ClMaterialLibResource.h"
#include "ClTaskResource.h"


using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(MaterialStockWarningDlg, CDialogEx)

MaterialStockWarningDlg::MaterialStockWarningDlg()
				: CLDialog(MaterialStockWarningDlg::IDD)
{
	m_strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MaterialStockWarning);
}

MaterialStockWarningDlg::~MaterialStockWarningDlg(void)
{
}

void MaterialStockWarningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MSG, m_strMsg);
	DDX_Control(pDX, IDC_LIST_MaterialNoEnoughStock, m_grid_MaterialNoEnoughStock);
}

BEGIN_MESSAGE_MAP(MaterialStockWarningDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()

    ON_BN_CLICKED(IDC_BTN_IncreaseStockAndSave, OnIncreaseStockAndSave)
    ON_BN_CLICKED(IDC_BTN_CancelSave, OnCancelSave)

END_MESSAGE_MAP()

BOOL MaterialStockWarningDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect r;
	GetClientRect(&r);
	if ((HBRUSH)m_brush == NULL)
		m_brush.CreateSolidBrush(UI_COLOR);
	pDC->FillRect(&r, &m_brush);

	return TRUE;
}

HBRUSH MaterialStockWarningDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	TCHAR classname[MAX_PATH];
	if (::GetClassName(pWnd->m_hWnd, classname, MAX_PATH) == 0)
		return hbr;

	pDC->SetBkColor(UI_COLOR);

	if ((HBRUSH)m_brush == NULL)
		m_brush.CreateSolidBrush(UI_COLOR);

	return (HBRUSH) m_brush;
}

BOOL MaterialStockWarningDlg::OnInitDialog()
{
	__super::OnInitDialog();

    CString str;

    // 设置表头
    {
        // Requires one never uses column 0
        m_grid_MaterialNoEnoughStock.InsertHiddenLabelColumn();

        // 序号
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
        CGridColumnTrait* pNUMCol1 = new CGridColumnTraitText;
        m_grid_MaterialNoEnoughStock.InsertColumnTrait(1, str, LVCFMT_LEFT, 34, 0, pNUMCol1);

        // 材料类型
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAT_Type);
        CGridColumnTrait* pMaterialTypeCol = new CGridColumnTraitText;
        m_grid_MaterialNoEnoughStock.InsertColumnTrait(2, str, LVCFMT_LEFT, 90, 0, pMaterialTypeCol);

        // 材料厚度
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAT_THICKNESS);
        CGridColumnTrait* pMaterialThicknessCol = new CGridColumnTraitText;
        m_grid_MaterialNoEnoughStock.InsertColumnTrait(3, str, LVCFMT_LEFT, 100, 0, pMaterialThicknessCol);

        // 材料尺寸
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAT_Size);
        CGridColumnTrait* pMaterialSizeCol = new CGridColumnTraitText;
        m_grid_MaterialNoEnoughStock.InsertColumnTrait(4, str, LVCFMT_LEFT, 90, 0, pMaterialSizeCol);

        // 可用数量
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAT_AVAILABLE_NUM);
        CGridColumnTrait* pAvailableNumCol = new CGridColumnTraitText;
        m_grid_MaterialNoEnoughStock.InsertColumnTrait(5, str, LVCFMT_LEFT, 90, 0, pAvailableNumCol);

        // 需要的数量
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_RequiredMaterialQuantity);
        CGridColumnTrait* pRequiredNumCol = new CGridColumnTraitText;
        m_grid_MaterialNoEnoughStock.InsertColumnTrait(6, str, LVCFMT_LEFT, 90, 0, pRequiredNumCol);
    }

    // 初始化表格内容
    {
        for (unsigned int i = 0; i < m_materialData_noEnoughStock.size(); i++)
        {
            const DataProperty& materialProperty = m_materialData_noEnoughStock[i].first;
            int iRequiredCount = m_materialData_noEnoughStock[i].second;

            // insert an item.
            int iRowCount = m_grid_MaterialNoEnoughStock.GetItemCount();
            m_grid_MaterialNoEnoughStock.InsertItem(iRowCount, _T(""));

            // 序号
            str.Format(_T("%d"), iRowCount + 1);
            m_grid_MaterialNoEnoughStock.SetItemText(iRowCount, 1, str);

            // 材料信息
            m_grid_MaterialNoEnoughStock.SetItemText(iRowCount, 2, materialProperty.GetItem(0).GetValue().bstrVal);
            str.Format(FLOAT_FORMAT_2, materialProperty.GetItem(1).GetValue().dblVal);
            m_grid_MaterialNoEnoughStock.SetItemText(iRowCount, 3, str);
            m_grid_MaterialNoEnoughStock.SetItemText(iRowCount, 4, materialProperty.GetItem(6).GetValue().bstrVal);
            str.Format(_T("%d"), materialProperty.GetItem(7).GetValue().dblVal);
            m_grid_MaterialNoEnoughStock.SetItemText(iRowCount, 5, str);
            str.Format(_T("%d"), iRequiredCount);
            m_grid_MaterialNoEnoughStock.SetItemText(iRowCount, 6, str);
        }
    }

	SetBackgroundColor(UI_COLOR);
	return TRUE;
}

void MaterialStockWarningDlg::OnIncreaseStockAndSave()
{
    EndDialog(IDC_BTN_IncreaseStockAndSave);
}

void MaterialStockWarningDlg::OnCancelSave()
{
    EndDialog(IDC_BTN_CancelSave);
}

END_CUTLEADER_NAMESPACE()
