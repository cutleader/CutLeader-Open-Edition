#include "stdafx.h"
#include "TaskPartManagementDlg.h"

#include "baseConst.h"
#include "LogMgr.h"
#include "ClBaseDataResource.h"
#include "DataCenterItemLoader.h"
#include "DataBaseManager.h"
#include "PathHelper.h"
#include "FolderNode.h"
#include "ClWindowFinder.h"

#include "ExpertLibItemLoader.h"
#include "ParamConfigLoader.h"
#include "CLOptionInfo.h"
#include "ImpExpConfig.h"

#include "PatternLoopList.h"
#include "Part.h"
#include "PartCadData.h"
#include "PartBuilder.h"
#include "DxfDwgConvertor.h"
#include "LoopProcessor.h"
#include "PartManager.h"
#include "Loop2PartDlg.h"
#include "PartsFromAsmDxf.h"
#include "PartList.h"
#include "SheetProcessor.h"
#include "SheetList.h"
#include "AddPartFromDB2TaskDlg.h"
#include "TaskItem.h"
#include "TaskPartList.h"
#include "TaskPart.h"
#include "TaskPartInfoDlg.h"
#include "ClTaskResource.h"
#include "ImportDxfDwg2TaskDlg.h"
#include "ClDataImportExportResource.h"
#include "TaskPartsSettingDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace std;


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TaskPartManagementDlg, CLResizableDialog)

TaskPartManagementDlg::TaskPartManagementDlg() : CLResizableDialog(TaskPartManagementDlg::IDD)
{
}

TaskPartManagementDlg::~TaskPartManagementDlg()
{
}

void TaskPartManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CLResizableDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_BTN_selectAll, m_selectAllButton);
	DDX_Control(pDX, IDC_BTN_AddPart2TaskFromDB, m_ctrlAddPart2TaskFromDbButton);
	DDX_Control(pDX, IDC_BTN_AddPart2TaskFromDxf, m_ctrlAddPart2TaskFromDxfButton);
	DDX_Control(pDX, IDC_BTN_EditTaskPartInfo, m_ctrlEditPartInfoButton);
	DDX_Control(pDX, IDC_BTN_deletePartFromTask, m_ctrlDeletePartFromTaskButton);
}

BEGIN_MESSAGE_MAP(TaskPartManagementDlg, CLResizableDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
    ON_WM_SIZE()

    ON_BN_CLICKED(IDC_BTN_AddPart2TaskFromDB, OnAddPart2TaskFromDB)
	ON_BN_CLICKED(IDC_BTN_AddPart2TaskFromDxf, OnAddPart2TaskFromDxf)
    ON_BN_CLICKED(IDC_BTN_EditTaskPartInfo, OnEditTaskPartInfo)
    ON_BN_CLICKED(IDC_BTN_deletePartFromTask, OnDeletePartFromTask)
    ON_BN_CLICKED(IDC_BTN_selectAll, OnSelectAll)

    ON_MESSAGE(WM_selectOrUnSelectPartInTaskPartDlg, OnSelectOrUnSelectPartInTaskPartDlg)
END_MESSAGE_MAP()

BOOL TaskPartManagementDlg::OnInitDialog()
{
	__super::OnInitDialog();

    // ���ֿؼ�
	AddAnchor(IDC_BTN_AddPart2TaskFromDB, TOP_LEFT);
	AddAnchor(IDC_BTN_AddPart2TaskFromDxf, TOP_LEFT);
	AddAnchor(IDC_BTN_EditTaskPartInfo, TOP_LEFT);
	AddAnchor(IDC_BTN_deletePartFromTask, TOP_LEFT);
	AddAnchor(IDC_BTN_selectAll, TOP_LEFT);
    AddAnchor(IDC_STATIC_TaskPartsArea, TOP_LEFT, BOTTOM_LEFT);
 
    m_selectAllButton.SetButtonStyle(BS_3STATE);

    // ��ʼ����ť
	{
		m_ctrlAddPart2TaskFromDbButton.SetImage(IDB_ADD_PART_FROM_DB_ToTask_24);
		m_ctrlAddPart2TaskFromDbButton.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AddTaskPartFromDataCenter));

		m_ctrlAddPart2TaskFromDxfButton.SetImage(IDB_ADD_PART_FROM_DXF_ToTask_24);
		m_ctrlAddPart2TaskFromDxfButton.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_AddTaskPartFromDxfDwg));

		m_ctrlEditPartInfoButton.SetImage(IDB_EditTaskParts_24);
		m_ctrlEditPartInfoButton.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_EditPartInfo));

		m_ctrlDeletePartFromTaskButton.SetImage(IDB_DELETE_PART_FromTask_24);
		m_ctrlDeletePartFromTaskButton.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_DeletePartFromTask));
	}

    // �����ӶԻ���
    {
        m_taskPartInfoListDlg.Create(IDD_clTaskUI_TaskPartInfoList, this);
        m_taskPartInfoListDlg.ShowWindow(SW_HIDE);
    }

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void TaskPartManagementDlg::OnDestroy()
{
	CLResizableDialog::OnDestroy();
}

HBRUSH TaskPartManagementDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void TaskPartManagementDlg::OnSize(UINT nType, int cx, int cy)
{
    CLResizableDialog::OnSize(nType, cx, cy);

    // �����ӶԻ���
    CWnd* pWndPos = GetDlgItem(IDC_STATIC_TaskPartsArea);
    if (pWndPos)
    {
        CRect rectClient;
        pWndPos->GetWindowRect(&rectClient);
        ScreenToClient(&rectClient);
        m_taskPartInfoListDlg.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
        m_taskPartInfoListDlg.ShowWindow(SW_SHOW);
    }
}

LRESULT TaskPartManagementDlg::OnSelectOrUnSelectPartInTaskPartDlg(WPARAM wParam, LPARAM lParam)
{
    // ���¸�ѡ��
    Update3StatusCheckBox();

    return 0;
}

void TaskPartManagementDlg::Update3StatusCheckBox()
{
    if (AreAllPartsSelected())
    {
        m_selectAllButton.SetCheck(BST_CHECKED);
    }
    else if (IsNonePartSelected())
    {
        m_selectAllButton.SetCheck(BST_UNCHECKED);
    }
    else
    {
        m_selectAllButton.SetCheck(BST_INDETERMINATE);
    }
}

void TaskPartManagementDlg::OnAddPart2TaskFromDB()
{
	AddPartFromDB2TaskDlg dlg;
	dlg.SetTaskItem(m_pTaskItem);
	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
	dlg.Init(DataCenterItemLoader::LoadProductTree(pMainDBCon));
	if (dlg.DoModal() == IDOK)
	{
		m_taskPartInfoListDlg.Redisplay_TaskPartInfoListDlg(m_pTaskItem);

		// ���¸�ѡ��
		Update3StatusCheckBox();
	}
}

void TaskPartManagementDlg::OnAddPart2TaskFromDxf()
{
	CString strFileName;
	ImportDxfDwg2TaskDlg dlg(this);
	int iBufferSize = 100 * 1000 * MAX_PATH;
	dlg.m_ofn.lpstrFile = strFileName.GetBuffer(iBufferSize);
	dlg.m_ofn.nMaxFile = iBufferSize;
	if (dlg.DoModal() == IDOK)
	{
		vector<CString> dxfdwgFullPaths;

		// ����ÿ��dxf/dwg�ļ��������ÿ��part��
		POSITION pos = dlg.GetStartPosition();
		while (pos != NULL)
		{
			CString strDxfDwgFilePath = dlg.GetNextPathName(pos);
			dxfdwgFullPaths.push_back(strDxfDwgFilePath);
		}

		// ��dxfdwg�������񣬲���¼���������¼���
		DBConnectPtr pExpertLibConnection = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		DataProperty paramConfigProperty;
		if (ParamConfigLoader::GetParamConfigInfo(pExpertLibConnection, dlg.GetParamConfigID(), paramConfigProperty))
		{
			const CString& strExpertLibItemName = paramConfigProperty.GetItem(3).GetValue().bstrVal;
			const CString& strParamConfigName = paramConfigProperty.GetItem(2).GetValue().bstrVal;
			const CString& strDataCenterPath = dlg.GetSavePath()->GetFullPath();
			AddDxfdwgToTask(dxfdwgFullPaths, strDataCenterPath, strExpertLibItemName, strParamConfigName);
		}
		else
		{
			LogMgr::DebugWarn(_T("216464"));
		}
	}
	strFileName.ReleaseBuffer();
}

void TaskPartManagementDlg::AddDxfdwgToTask(const std::vector<CString>& dxfdwgFullPaths, const CString& strDataCenterPath,
	const CString& strExpertLibItemName, const CString& strParamConfigName)
{
	DBConnectPtr pExpertLibConnection = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	LONGLONG iExpertLibItemID = ExpertLibItemLoader::GetExpLibItemIDByName(pExpertLibConnection, strExpertLibItemName);
	LONGLONG iParamConfigID = ParamConfigLoader::GetConfigIDByName(pExpertLibConnection, iExpertLibItemID, strParamConfigName);

	vector<PartPtr> newPartList;
	map<LONGLONG, int> partCount; // �����������ĳ�������dxf���ж���ұ��ϲ��˺󣬲��������ֵ���϶�����1��

	for (unsigned int i = 0; i < dxfdwgFullPaths.size(); i++)
	{
		const CString& strDxfDwgFilePath = dxfdwgFullPaths[i];

		// ����dxf/dwg�ļ�ΪPartCadData���󣬴�ʱ��Ҫ�������л�·�������Ӱ�����˹�ϵʶ��
		BOOL bIsDwg = (PathHelper::GetFileExt(strDxfDwgFilePath).CompareNoCase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DWG_EXT)) == 0 ? TRUE : FALSE);
		PartCadDataPtr pPartCadData = DxfDwgConvertor::DxfDwg2CadData(strDxfDwgFilePath, bIsDwg);

		// ������������˹�ϵ�����з��������������
		if (!LoopProcessor::NeedToSeperate(LoopProcessor::CalcLoopTopStruct(pPartCadData->GetPatternLoopList().get()).get()))
		{
			UINT iAssignLimit = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetAssignLimit();
			PartPtr pPart = PartBuilder::BuildPart(PathHelper::GetFileName(strDxfDwgFilePath), pPartCadData, iParamConfigID, iAssignLimit, TRUE);
			newPartList.push_back(pPart);
		}
		else
		{
			// ����Ի������ڴӻ�·ʶ�������
			Loop2PartDlg loop2PartDlg;
			loop2PartDlg.Init(pPartCadData, strDxfDwgFilePath, iParamConfigID);
			if (loop2PartDlg.DoModal() == IDC_BTN_ACCEPT)
			{
				PartsFromAsmDxfPtr pPartsFromAsmDxf = loop2PartDlg.Get_PartsFromAsmDxf();
				const map<LONGLONG, vector<pair<LONGLONG, Matrix2D>>>& matrixData_of_sameShapePart = pPartsFromAsmDxf->GetMatrixsOfSameShapePart();
				for (unsigned int i = 0; i < pPartsFromAsmDxf->GetPartList()->size(); i++)
				{
					PartPtr pPart = pPartsFromAsmDxf->GetPartList()->operator [](i);
					newPartList.push_back(pPart);

					map<LONGLONG, vector<pair<LONGLONG, Matrix2D>>>::const_iterator iter = matrixData_of_sameShapePart.find(pPart->GetID());
					if (iter != matrixData_of_sameShapePart.end())
					{
						partCount[pPart->GetID()] = iter->second.size();
					}
				}
			}
		}
	}

	// ������������ݿ⡣
	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
	FolderNodePtr pDataCenterRootFolder = DataCenterItemLoader::LoadProductTree(pMainDBCon);
	IDataCenterItemPtr pDataCenterItem = pDataCenterRootFolder->SearchDataCenterItemByFullPath(strDataCenterPath);
	if (!pDataCenterItem)
	{
		LogMgr::DebugWarn(_T("540213"));
		return;
	}
	FolderNodePtr pFolderItem = boost::dynamic_pointer_cast<FolderNode>(pDataCenterItem);
	if (!pFolderItem)
	{
		LogMgr::DebugWarn(_T("621017"));
		return;
	}
	PartManager::SaveNewPartList(newPartList, pFolderItem);

	// ���������������
	for (unsigned int i = 0; i < newPartList.size(); i++)
	{
		PartPtr pPart = newPartList[i];
		TaskPartPtr pTaskPart(new TaskPart(pPart, NestPriority::MaxPriority(), PartRotate_Free, 1));
		m_pTaskItem->GetTaskPartList()->push_back(pTaskPart);

		// Ҫ����������dxf���ж�����ұ��ϲ��ˣ������������Ӧ�ô���1��
		map<LONGLONG, int>::const_iterator iter = partCount.find(pPart->GetID());
		if (iter != partCount.end())
		{
			pTaskPart->SetPlanningCount(iter->second + 1);
		}
	}

	// ˢ���������
	m_taskPartInfoListDlg.Redisplay_TaskPartInfoListDlg(m_pTaskItem);

	// ���¸�ѡ��
	Update3StatusCheckBox();
}

void TaskPartManagementDlg::OnEditTaskPartInfo()
{
    TaskPartsSettingDlg dlg;
    dlg.SetTaskItem(m_pTaskItem);
    if (dlg.DoModal() == IDC_BTN_OK)
    {
        // ������ʾ������������ӶԻ���
        m_taskPartInfoListDlg.Redisplay_TaskPartInfoListDlg(m_pTaskItem);
    }
}

void TaskPartManagementDlg::OnDeletePartFromTask()
{
    CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

    // �õ�����ѡ�е����������
    TaskPartListPtr pSelectedTaskParts = GetSelectedTaskParts();
    if (pSelectedTaskParts->size() == 0)
    {
        CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NoPartIsSelectedToDelete);
        MessageBox(str, strProductName, MB_ICONWARNING);
        return;
    }

    // ִ��ɾ��
    {
        // ��һ��ѡ�е�����ǲ����е��Ѿ��ڰ�������ˡ�
        bool bSomePartHasOnSheet = false;
        for (unsigned int i = 0; i < pSelectedTaskParts->size(); i++)
        {
            const TaskPart* pTaskPart = pSelectedTaskParts->operator [](i).get();
            int iNestedCount = m_pTaskItem->GetSheetList()->GetPartNestedCount(pTaskPart->GetPart()->GetID());
            if (iNestedCount > 0)
            {
                bSomePartHasOnSheet = true;
                break;
            }
        }

        // �Ƿ����Ҫɾ�������
        bool bMakeSureToDeleteParts = false;
        if (bSomePartHasOnSheet)
        {
            CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MakeSureToDeletePart_2);
            if (MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES)
            {
                bMakeSureToDeleteParts = true;
            }
        }
        else
        {
            CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MakeSureToDeletePart);
            if (MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES)
            {
                bMakeSureToDeleteParts = true;
            }
        }

        //
        if (bMakeSureToDeleteParts)
        {
            // �ȴӰ����ɾ�������
            if (bSomePartHasOnSheet)
            {
                for (unsigned int i = 0 ; i < pSelectedTaskParts->size(); i++)
                {
                    const Part* pPart = pSelectedTaskParts->operator [](i)->GetPart();
                    for (unsigned int j = 0; j < m_pTaskItem->GetSheetList()->size(); j++)
                    {
                        SheetPtr pSheet = m_pTaskItem->GetSheetList()->operator [](j);
                        SheetProcessor::DeletePartFromSheet(pSheet, pPart->GetID());
                    }
                }
            }

            // ��������ɾ�������
            for (unsigned int i = 0; i < pSelectedTaskParts->size(); i++)
            {
                m_pTaskItem->DeletePartFromTask(pSelectedTaskParts->operator [](i)->GetID());
                m_pTaskItem->SetModified(TRUE);
            }

            // ������ʾ������������ӶԻ���
            m_taskPartInfoListDlg.Redisplay_TaskPartInfoListDlg(m_pTaskItem);

            // ���ѡ��Ķ���
            std::vector<TaskPartInfoDlgPtr> taskPartSubDlgs = m_taskPartInfoListDlg.GetTaskPartSubDlgs();
            for (unsigned int i = 0; i < taskPartSubDlgs.size(); i++)
            {
                TaskPartInfoDlgPtr pTaskPartSubDlg = taskPartSubDlgs.operator [](i);
                pTaskPartSubDlg->UnSelectThisTaskPart();
            }

            // ���¸�ѡ��
            Update3StatusCheckBox();

            //
            ClWindowFinder::GetCurrentView_2()->SendMessage(WM_reDisplayTaskSheets, 0, 0);
        }
    }
}

void TaskPartManagementDlg::OnSelectAll()
{
    if (m_selectAllButton.GetCheck() == BST_UNCHECKED || m_selectAllButton.GetCheck() == BST_INDETERMINATE)
    {
        // ѡ�����е����
        std::vector<TaskPartInfoDlgPtr> taskPartSubDlgs = m_taskPartInfoListDlg.GetTaskPartSubDlgs();
        for (unsigned int i = 0; i < taskPartSubDlgs.size(); i++)
        {
            TaskPartInfoDlgPtr pTaskPartSubDlg = taskPartSubDlgs.operator [](i);
            pTaskPartSubDlg->SelectThisTaskPart();
        }

        m_selectAllButton.SetCheck(BST_CHECKED);
    }
    else if (m_selectAllButton.GetCheck() == BST_CHECKED)
    {
        // ���ѡ��Ķ���
        std::vector<TaskPartInfoDlgPtr> taskPartSubDlgs = m_taskPartInfoListDlg.GetTaskPartSubDlgs();
        for (unsigned int i = 0; i < taskPartSubDlgs.size(); i++)
        {
            TaskPartInfoDlgPtr pTaskPartSubDlg = taskPartSubDlgs.operator [](i);
            pTaskPartSubDlg->UnSelectThisTaskPart();
        }

        m_selectAllButton.SetCheck(BST_UNCHECKED);
    }
}

bool TaskPartManagementDlg::AreAllPartsSelected() const
{
    std::vector<TaskPartInfoDlgPtr> taskPartSubDlgs = m_taskPartInfoListDlg.GetTaskPartSubDlgs();
    for (unsigned int i = 0; i < taskPartSubDlgs.size(); i++)
    {
        TaskPartInfoDlgPtr pTaskPartSubDlg = taskPartSubDlgs.operator [](i);
        if (pTaskPartSubDlg->IsWindowVisible() && !pTaskPartSubDlg->IsSelected())
        {
            return false;
        }
    }

    return true;
}

bool TaskPartManagementDlg::IsNonePartSelected() const
{
    std::vector<TaskPartInfoDlgPtr> taskPartSubDlgs = m_taskPartInfoListDlg.GetTaskPartSubDlgs();
    for (unsigned int i = 0; i < taskPartSubDlgs.size(); i++)
    {
        TaskPartInfoDlgPtr pTaskPartSubDlg = taskPartSubDlgs.operator [](i);
        if (pTaskPartSubDlg->IsWindowVisible() && pTaskPartSubDlg->IsSelected())
        {
            return false;
        }
    }

    return true;
}

void TaskPartManagementDlg::Redisplay_TaskPartManagementDlg(TaskItemPtr pTaskItem)
{
    m_pTaskItem = pTaskItem;
    m_taskPartInfoListDlg.Redisplay_TaskPartInfoListDlg(pTaskItem);
    //this->Invalidate(TRUE);//�о�����Ҫ�������
}

void TaskPartManagementDlg::ReDisplayNestedCount_ofEachPart()
{
    if (!m_pTaskItem) // ���������������
    {
        LogMgr::DebugWarn(_T("2958"));
    }

    m_taskPartInfoListDlg.ReDisplayNestedCount_ofEachPart();
}

TaskItemPtr TaskPartManagementDlg::GetTaskItem() const
{
    if (!m_pTaskItem) // ���������������
    {
        LogMgr::DebugWarn(_T("2ks8"));
    }

    return m_pTaskItem;
}

TaskPartListPtr TaskPartManagementDlg::GetSelectedTaskParts() const
{
	TaskPartListPtr pSelectedTaskParts(new TaskPartList);

	std::vector<TaskPartInfoDlgPtr> taskPartSubDlgs = m_taskPartInfoListDlg.GetTaskPartSubDlgs();
	for (unsigned int i = 0; i < taskPartSubDlgs.size(); i++)
	{
		TaskPartInfoDlgPtr pTaskPartSubDlg = taskPartSubDlgs.operator [](i);
		if (pTaskPartSubDlg->IsWindowVisible() && pTaskPartSubDlg->IsSelected())
		{
			pSelectedTaskParts->push_back(pTaskPartSubDlg->GetTaskPart());
		}
	}

	return pSelectedTaskParts;
}

END_CUTLEADER_NAMESPACE()
