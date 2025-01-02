#include "stdafx.h"
#include "MaterialSizeInfoPage.h"

#include "baseConst.h"
#include "MathEx.h"
#include "clUtilityResource.h"
#include "CGridColumnManagerProfile.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridRowTraitXP.h"

#include "LineArc2DList.h"
#include "baseConst.h"
#include "clBaseDataResource.h"
#include "clMaterialLibResource.h"
#include "MaterialProcessor.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(MaterialSizeInfoPage, CResizableDialog)

MaterialSizeInfoPage::MaterialSizeInfoPage() : CLResizableDialog(MaterialSizeInfoPage::IDD)
{
	m_pMatSize.reset();
}

MaterialSizeInfoPage::~MaterialSizeInfoPage()
{
}

void MaterialSizeInfoPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	// basic info group.
	DDX_Text(pDX, IDC_EDIT_MATSIZE_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_MATSIZE_COMMENT, m_strComment);
	DDX_Control(pDX, IDC_COMBO_IS_REMNANT, m_ctrlIsRemnant);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_dWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_dHeight);

	// "stock info" group.
	DDX_Control(pDX, IDC_EDIT_MAT_COUNT, m_iCount);
	DDX_Control(pDX, IDC_EDIT_MAT_USED, m_iUsed);

	DDX_Control(pDX, IDC_STATIC_PREVIEW_MAT, m_matPreviewWnd);
	
	DDX_Control(pDX, IDC_BTN_ROT_90, m_btnROT);
}

BEGIN_MESSAGE_MAP(MaterialSizeInfoPage, CResizableDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()

	ON_EN_KILLFOCUS(IDC_EDIT_MATSIZE_NAME, OnKillName)
	ON_EN_KILLFOCUS(IDC_EDIT_MATSIZE_COMMENT, OnKillComment)
	ON_EN_KILLFOCUS(IDC_EDIT_WIDTH, OnKillWidth)
	ON_EN_KILLFOCUS(IDC_EDIT_HEIGHT, OnKillHeight)
	ON_EN_KILLFOCUS(IDC_EDIT_MAT_COUNT, OnKillCount)

	ON_BN_CLICKED(IDC_BTN_ROT_90, OnCCWRot)
END_MESSAGE_MAP()

BOOL MaterialSizeInfoPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	/************************************************************************/
	// layout controls.

	// "basic info" group
	AddAnchor(IDC_STATIC_BASE_INFO, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_STATIC_MATSIZE_NAME, TOP_LEFT);
	AddAnchor(IDC_EDIT_MATSIZE_NAME, TOP_LEFT);
	AddAnchor(IDC_STATIC_NAME, TOP_LEFT);
	AddAnchor(IDC_EDIT_WIDTH, TOP_LEFT);
	AddAnchor(IDC_STATIC_NAME2, TOP_LEFT);
	AddAnchor(IDC_EDIT_HEIGHT, TOP_LEFT);
	AddAnchor(IDC_STATIC_NAME5, TOP_LEFT);
	AddAnchor(IDC_COMBO_IS_REMNANT, TOP_LEFT);
	AddAnchor(IDC_STATIC_COMMENT, TOP_LEFT);
	AddAnchor(IDC_EDIT_MATSIZE_COMMENT, TOP_LEFT);

	// "stock info" group
	AddAnchor(IDC_STATIC_BASE_INFO3, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_STATIC_NAME3, TOP_LEFT);
	AddAnchor(IDC_EDIT_MAT_COUNT, TOP_LEFT);
	AddAnchor(IDC_STATIC_NAME4, TOP_LEFT);
	AddAnchor(IDC_EDIT_MAT_USED, TOP_LEFT);

	// preview group.
	AddAnchor(IDC_GROUP_PREVIEW_MAT, TOP_LEFT);
	AddAnchor(IDC_STATIC_PREVIEW_MAT, TOP_LEFT);

	AddAnchor(IDC_BTN_ROT_90, TOP_LEFT);
	/************************************************************************/

	CString str;
	m_ctrlIsRemnant.ResetContent();
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_YES);
	m_ctrlIsRemnant.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NO);
	m_ctrlIsRemnant.AddString(str);

	// 初始化视口。
	Rect2D geomRect(.0, 10.0, .0, 10.0);
	m_matPreviewWnd.InitViewPort(geomRect);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_BASE_INFO )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_BASE_INFO3 )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_PREVIEW_MAT )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

void MaterialSizeInfoPage::OnPaint()
{
	CResizableDialog::OnPaint();
	PreviewMatSize();
}

void MaterialSizeInfoPage::OnDestroy()
{
	// 释放视口。
	m_matPreviewWnd.ReleaseViewPort();

	CResizableDialog::OnDestroy();
}

HBRUSH MaterialSizeInfoPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void MaterialSizeInfoPage::OnKillName()
{
	UpdateData(TRUE);

	if (m_pMatSize)
	{
		if (m_pMatSize->GetName() != m_strName)
		{
			m_pMatSize->SetName(m_strName);
			m_pMatSize->SetModified(TRUE);
		}
	}
}

void MaterialSizeInfoPage::OnKillComment()
{
	UpdateData(TRUE);

	if (m_pMatSize)
	{
		if (m_pMatSize->GetComment() != m_strComment)
		{
			m_pMatSize->SetComment(m_strComment);
			m_pMatSize->SetModified(TRUE);
		}
	}
}

void MaterialSizeInfoPage::OnKillWidth()
{
	UpdateData(TRUE);

	if (m_pMatSize)
	{
		if (!MathEx::Equal(m_pMatSize->GetWidth(), m_dWidth.Get_Number(), GEOM_TOLERANCE))
		{
			m_pMatSize->SetWidth(m_dWidth.Get_Number());
			m_pMatSize->SetModified(TRUE);
		}
	}
}

void MaterialSizeInfoPage::OnKillHeight()
{
	UpdateData(TRUE);

	if (m_pMatSize)
	{
		if (!MathEx::Equal(m_pMatSize->GetHeight(), m_dHeight.Get_Number(), GEOM_TOLERANCE))
		{
			m_pMatSize->SetHeight(m_dHeight.Get_Number());
			m_pMatSize->SetModified(TRUE);
		}
	}
}

void MaterialSizeInfoPage::OnKillCount()
{
	UpdateData(TRUE);

	if (m_pMatSize)
	{
		// 材料总数要大于消耗的数量。
		if ((int)m_iCount.Get_Number() < (m_pMatSize->GetTotalNum() - m_pMatSize->GetAvailNum()))
		{
			// restore UI.
			m_iCount.Init(m_pMatSize->GetTotalNum(), 0, FALSE);
			UpdateData(FALSE);

			// prompt user.
			CString strProductName, strMsgContent;
			strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAT_SIZE_CHECK);
			MessageBox(strMsgContent, strProductName, MB_OK | MB_ICONINFORMATION);
			return;
		}

		if (m_pMatSize->GetTotalNum() != (int)m_iCount.Get_Number())
		{
			m_pMatSize->SetTotalNum((int)m_iCount.Get_Number());
			m_pMatSize->SetAvailNum((int)m_iCount.Get_Number() - (int)m_iUsed.Get_Number());
			m_pMatSize->SetModified(TRUE);
		}
	}
}

void MaterialSizeInfoPage::OnCCWRot()
{
	// 旋转。
	MaterialProcessor::Rot90Degree(m_pMatSize);
	m_pMatSize->SetModified(TRUE);

	// 重新显示。
	DisplayMatSize(m_pMatSize);
}

void MaterialSizeInfoPage::DisplayMatSize(IMaterialSizePtr pMaterialSize)
{
	m_pMatSize = pMaterialSize;

	m_strName = pMaterialSize->GetName();
	m_strComment = pMaterialSize->GetComment();
	UpdateData(FALSE);

	// the MatSize type.
	if (pMaterialSize->IsRemnant())
	{
		m_ctrlIsRemnant.SetCurSel(0);
	}
	else
	{
		m_ctrlIsRemnant.SetCurSel(1);
	}
	m_ctrlIsRemnant.Invalidate(TRUE);

	m_dWidth.Init(pMaterialSize->GetWidth(), DIGITAL_NUM, FALSE);
	m_dHeight.Init(pMaterialSize->GetHeight(), DIGITAL_NUM, FALSE);
	m_iCount.Init(pMaterialSize->GetTotalNum(), 0, FALSE);
	m_iUsed.Init((pMaterialSize->GetTotalNum()-pMaterialSize->GetAvailNum()), 0, FALSE);

	// 预览材料。
	PreviewMatSize();
}

void MaterialSizeInfoPage::PreviewMatSize()
{
	if (m_pMatSize)
	{
		// 初始化视口。
		LineArc2DListPtr pOutGeoms = MaterialProcessor::GetBorderLineArcs(m_pMatSize.get());
		Rect2D outRect = pOutGeoms->GetRectBox();
		m_matPreviewWnd.FitRect(outRect);

		// 设置材料外框。
		m_matPreviewWnd.SetSolidGeom2DList(pOutGeoms);

		// 绘制材料的无效区域。
		m_matPreviewWnd.SetDashGeom2DList(MaterialProcessor::GetUselessRegions(m_pMatSize.get()));

		m_matPreviewWnd.Invalidate();
	}
}

END_CUTLEADER_NAMESPACE()
