#include "stdafx.h"
#include "TaskPartsSettingDlg.h"

#include "ClUtilityResource.h"
#include "CGridColumnManagerProfile.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "ImgData.h"
#include "baseConst.h"
#include "ClBaseDataResource.h"
#include "DataBaseManager.h"
#include "StringUtil.h"

#include "BatchEditTaskPartDlg.h"
#include "TaskPartList.h"
#include "TaskPart.h"
#include "TaskItem.h"
#include "Part.h"
#include "ClPartResource.h"
#include "ClNestResource.h"
#include "NestPriority.h"
#include "PartLoader.h"
#include "NestPart.h"
#include "ClTaskResource.h"


using namespace std;
using namespace std::tr1;


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(TaskPartsSettingDlg, CLDialog)

BEGIN_MESSAGE_MAP(TaskPartsSettingDlg, CLDialog)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TaskPart, OnPartListchanged)
	ON_BN_CLICKED(IDC_BTN_BatchSetting, OnBatchSetting)

    ON_BN_CLICKED(IDC_BTN_OK, OnClickOk)
    ON_BN_CLICKED(IDC_BTN_CANCEL, OnClickCancel)
END_MESSAGE_MAP()

TaskPartsSettingDlg::TaskPartsSettingDlg(CWnd* pParent /*=NULL*/)
		: CLDialog(TaskPartsSettingDlg::IDD, pParent)
{
    m_bFinishAddNestPart = TRUE;
}

TaskPartsSettingDlg::~TaskPartsSettingDlg()
{
}

void TaskPartsSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CLDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_TaskPart, m_tableTaskParts);
	DDX_Control(pDX, IDC_STATIC_PartPreview, m_staticPartPreview);

	DDX_Control(pDX, IDC_BTN_BatchSetting, m_btnBatchSetting);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
}

BOOL TaskPartsSettingDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// ��ʼ�������ʽ��
	InitPartGridStyle();

    // ��ʼ���������
    const TaskPartList* pTaskPartList = m_pTaskItem->GetTaskPartList().get();
    for (unsigned int i = 0; i < pTaskPartList->size(); i++)
    {
        const TaskPart* pTaskPart = pTaskPartList->operator [](i).get();
        const Part* pPart = pTaskPart->GetPart();

        int iRowCount = m_tableTaskParts.GetItemCount();
        m_tableTaskParts.InsertItem(iRowCount, _T(""));
        m_tableTaskParts.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pPart->GetID())));
        m_tableTaskParts.SetItemText(iRowCount, 1, StringUtil::Integer2String(i + 1));
        m_tableTaskParts.SetItemText(iRowCount, 2, pPart->GetName());
        m_tableTaskParts.SetItemText(iRowCount, 3, StringUtil::Integer2String(pTaskPart->GetPlanningCount()));
        m_tableTaskParts.SetItemText(iRowCount, 4, StringUtil::Integer2String(pTaskPart->GetNestPriority().GetVal()));
        m_tableTaskParts.SetItemText(iRowCount, 5, NestPart::GetName_ofPartRotateStyle(pTaskPart->GetPartRotateType()));
    }

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void TaskPartsSettingDlg::OnDestroy()
{
	RlsPartGridData();

	CLDialog::OnDestroy();
}

void TaskPartsSettingDlg::OnPaint()
{
	PreviewPart();
	CLDialog::OnPaint();
}

HBRUSH TaskPartsSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void TaskPartsSettingDlg::OnBatchSetting()
{
	// �����û��ѡ�������
	int iSelCount = 0;
	for (int i = 0; i < m_tableTaskParts.GetItemCount(); i++)
	{
		if (m_tableTaskParts.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
        {
			iSelCount++;
        }
	}
	if (iSelCount == 0)
	{
		CString strProductName, str;
		strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NoPartIsSelectedToEdit);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	// �������������������Ϣ��
	BatchEditTaskPartDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		// �ƻ�����
		if (dlg.IsEnablePlanningCount())
		{
			int iPlanningCount = dlg.GetPlanningCount();
			for (int i = 0; i < m_tableTaskParts.GetItemCount(); i++)
			{
				if (m_tableTaskParts.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
				{
					m_tableTaskParts.SetItemText(i, 3, StringUtil::Integer2String(iPlanningCount));
				}
			}
		}

		// �Զ��Ű���ת�Ƕȡ�
		if (dlg.IsEnableNestingRotateAngle())
		{
			PartRotateType emNestingRotateAngle = dlg.GetNestingRotateAngle();
		    for (int i = 0; i < m_tableTaskParts.GetItemCount(); i++)
		    {
			    if (m_tableTaskParts.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
                {
				    m_tableTaskParts.SetItemText(i, 5, NestPart::GetName_ofPartRotateStyle(emNestingRotateAngle));
                }
		    }
		}
	}
}

void TaskPartsSettingDlg::OnPartListchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (!m_bFinishAddNestPart)
		return;

	// preview
	PreviewPart();

	*pResult = 0;
}

void TaskPartsSettingDlg::OnClickOk()
{
    TaskPartListPtr pTaskPartList = m_pTaskItem->GetTaskPartList();
    for (int i = 0; i < m_tableTaskParts.GetItemCount(); i++)
    {
        LONGLONG* pPartID = (LONGLONG*)m_tableTaskParts.GetItemData(i);
        TaskPartPtr pTaskPart = m_pTaskItem->GetTaskPartByPartID(*pPartID);

        int iPlanningCount = _wtoi(m_tableTaskParts.GetItemText(i, 3));
        if (pTaskPart->GetPlanningCount() != iPlanningCount)
        {
            pTaskPart->SetPlanningCount(iPlanningCount);
            m_pTaskItem->SetModified(TRUE);
        }

        int iNestPriority = _wtoi(m_tableTaskParts.GetItemText(i, 4));
        if (pTaskPart->GetNestPriority().GetVal() != iNestPriority)
        {
            pTaskPart->SetNestPriority(NestPriority(iNestPriority));
            m_pTaskItem->SetModified(TRUE);
        }

		PartRotateType emPartRotate = NestPart::GetPartRotateStyle_ofName(m_tableTaskParts.GetItemText(i, 5));
        if (pTaskPart->GetPartRotateType() != emPartRotate)
        {
            pTaskPart->SetPartRotateType(emPartRotate);
            m_pTaskItem->SetModified(TRUE);
        }
    }

    EndDialog(IDC_BTN_OK);
}

void TaskPartsSettingDlg::OnClickCancel()
{
    EndDialog(IDC_BTN_CANCEL);
}

void TaskPartsSettingDlg::InitPartGridStyle()
{
	// Requires one never uses column 0
	m_tableTaskParts.InsertHiddenLabelColumn();

	// the num column.
	CGridColumnTrait* pNUMCol1 = new CGridColumnTraitText;
	m_tableTaskParts.InsertColumnTrait(1, MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM), LVCFMT_LEFT, 40, 0, pNUMCol1);

	// the part name column.
	CGridColumnTrait* pNameCo2 = new CGridColumnTraitText;
	m_tableTaskParts.InsertColumnTrait(2, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTNAME), LVCFMT_LEFT, 120, 0, pNameCo2);

    // �ƻ�����
    CGridColumnTrait* pNameCo4 = new CGridColumnTraitEdit(INTEDITBOX);
    m_tableTaskParts.InsertColumnTrait(3, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PartPlanningCount), LVCFMT_LEFT, 100, 0, pNameCo4);

	// �Ű����ȼ�
	CGridColumnTraitCombo* pNameCo3 = new CGridColumnTraitCombo;
	pNameCo3->SetStyle(pNameCo3->GetStyle() | CBS_DROPDOWNLIST);
	StringListPtr pList1 = NestPriority::GetAllPriority();
	for (unsigned int i = 0; i < pList1->size(); i++)
    {
		pNameCo3->AddItem(i, pList1->at(i));
    }
	m_tableTaskParts.InsertColumnTrait(4, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PartNestingPriority), LVCFMT_LEFT, 140, 0, pNameCo3);

	// �Ű���ת�Ƕ�
	CGridColumnTraitCombo* pRotAngCol = new CGridColumnTraitCombo;
	pRotAngCol->SetStyle(pRotAngCol->GetStyle() | CBS_DROPDOWNLIST);
	vector<CString> allRotateStyles = NestPart::GetAllRotateStyles();
    for (unsigned int i = 0; i < allRotateStyles.size(); i++)
    {
        pRotAngCol->AddItem(i, allRotateStyles[i]);
    }
	m_tableTaskParts.InsertColumnTrait(5, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PartNestingRotateAngle), LVCFMT_LEFT, 170, 0, pRotAngCol);
}

void TaskPartsSettingDlg::PreviewPart()
{
	// �õ�ѡ���
	int iIndex = INVALID_IDX;
	for (int i = 0; i < m_tableTaskParts.GetItemCount(); i++)
	{
		if (m_tableTaskParts.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED)
        {
			iIndex = i;
        }
	}

	// �õ�Ԥ�����ݡ�
	ImgDataPtr pImgData;
	pImgData.reset();
	if (iIndex != INVALID_IDX)
	{
		LONGLONG* pPartID = (LONGLONG*)m_tableTaskParts.GetItemData(iIndex);
		DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(*pPartID);
		pImgData = PartLoader::LoadPreviewData(pPartDBCon);
	}

	// Ԥ��
	ImgData::DrawImgData(pImgData.get(), &m_staticPartPreview, SkinManager::GetBackgroundColor());
}

void TaskPartsSettingDlg::RlsPartGridData()
{
	for (int i = 0; i < m_tableTaskParts.GetItemCount(); i++)
	{
		DWORD_PTR pItemData = m_tableTaskParts.GetItemData(i);
		delete (LONGLONG*)pItemData;
	}
}

END_CUTLEADER_NAMESPACE()
