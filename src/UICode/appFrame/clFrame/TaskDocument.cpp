#include "stdafx.h"

#include "DBConnect.h"
#include "DataBaseManager.h"
#include "DataCenterItemLoader.h"
#include "ClBaseDataResource.h"
#include "FolderNode.h"
#include "DateHelper.h"
#include "DataItemTracker.h"
#include "RecentDataList.h"
#include "RecentData.h"
#include "LogMessage.h"
#include "DataSaveInfoDlg.h"
#include "LogMgr.h"

#include "TaskDocument.h"
#include "TaskItem.h"
#include "ClTaskResource.h"
#include "TaskItemManager.h"
#include "MaterialStockWarningDlg.h"
#include "ClFrameResource.h"
#include "UIManager.h"


using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TaskDocument, CLeaderDoc)

BEGIN_MESSAGE_MAP(TaskDocument, CLeaderDoc)
END_MESSAGE_MAP()


TaskDocument::TaskDocument()
{
}

TaskDocument::~TaskDocument()
{
}

BOOL TaskDocument::OnNewDocument()
{
	if (!CLeaderDoc::OnNewDocument())
		return FALSE;

	return TRUE;
}

BOOL TaskDocument::OnSaveDocument()
{
    CString strProductName, strMsgContent, strTaskName;
    strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_TASK);
    strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

    if (m_pTaskItem->IsModified())
    {
        // 找出没有足够库存的材料。
        vector<pair<DataProperty, int>> materialData_noEnoughStock = TaskItemManager::GetMaterialData_noEnoughStock(m_pTaskItem.get());
        if (materialData_noEnoughStock.size() > 0) // 显示对话框让用户选择。
        {
            MaterialStockWarningDlg dlg;
            dlg.Set_MaterialData_NoEnoughStock(materialData_noEnoughStock);
            if (dlg.DoModal() == IDC_BTN_CancelSave)
            {
                return FALSE;
            }
        }

        // 是否是新任务。
        DataProperty taskProp;
        DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
        if (!DataCenterItemLoader::GetDataProp(pMainDBCon, m_pTaskItem->GetID(), taskProp))
        {
            // choose where to save.
            DataSaveInfoDlg dlg;
            dlg.Init(strTaskName, DataCenterItemLoader::LoadProductTree(pMainDBCon), m_pTaskItem->GetName());
            if (dlg.DoModal() == IDOK)
            {
                FolderNodePtr pParentFolder = dlg.GetSelectFolder();
                CString strName = dlg.GetName();
                m_pTaskItem->SetName(strName);

                // update the date.
                m_pTaskItem->SetCreateTime(DateHelper::GetCurrentTime());
                m_pTaskItem->SetModifyTime(DateHelper::GetCurrentTime());

                // 保存新任务
                DataItemPtr pDataItem = TaskItemManager::SaveNewTaskItem(m_pTaskItem, pParentFolder);

                // 修改标题
                SetTitle(strName);

                // update the tracked info.
                {
                    // update the current opened data.
                    DataItemTracker::GetInstance()->AddDataItem(pDataItem);

                    // update data which opened recently.
                    RecentDataListPtr pRecentDataList = DataItemTracker::GetInstance()->GetRecentData();
                    RecentDataPtr pRecentData(new RecentData(pDataItem->GetFullPath(), pDataItem->GetDataID()));
                    pRecentDataList->insert(pRecentDataList->begin(), pRecentData);
                    UIManager::GetInstance()->UpdateRecentDataMenu();
                }

                // 弹出消息
                {
                    strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVED);
                    strMsgContent.Replace(_T("WILLBEREPLACED1"), m_pTaskItem->GetName());
                    LogMessagePtr pLogMsg(new LogMessage(DateHelper::GetCurrentTime(), LOG_MSG_INFO, strTaskName, strMsgContent));
                    LogMgr::WriteLog(pLogMsg);
                    POSITION pos = GetFirstViewPosition();
                    MessageBox(GetNextView(pos)->m_hWnd, strMsgContent, strProductName, MB_OK | MB_ICONINFORMATION);
                }
            }
        }
        else
        {
            // save the existing task.
            TaskItemManager::SaveExistTaskItem(m_pTaskItem);

            // update data which opened recently.
            {
                RecentDataListPtr pRecentDataList = DataItemTracker::GetInstance()->GetRecentData();
                RecentDataPtr pRecentData = pRecentDataList->GetItemByDataID(m_pTaskItem->GetID());
                pRecentDataList->MoveToHead(pRecentData->GetID());
                UIManager::GetInstance()->UpdateRecentDataMenu();
            }

            // create a message.
            {
                strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVED);
                strMsgContent.Replace(_T("WILLBEREPLACED1"), m_pTaskItem->GetName());
                LogMessagePtr pLogMsg(new LogMessage(DateHelper::GetCurrentTime(), LOG_MSG_INFO, strTaskName, strMsgContent));
                LogMgr::WriteLog(pLogMsg);
                POSITION pos = GetFirstViewPosition();
                MessageBox(GetNextView(pos)->m_hWnd, strMsgContent, strProductName, MB_OK | MB_ICONINFORMATION);
            }
        }
    }
    else
    {
        // create a message.
        strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_NOT_CHANGED);
        strMsgContent.Replace(_T("WILLBEREPLACED1"), m_pTaskItem->GetName());
        LogMessagePtr pLogMsg(new LogMessage(DateHelper::GetCurrentTime(), LOG_MSG_WARNING, strTaskName, strMsgContent));
        LogMgr::WriteLog(pLogMsg);
        POSITION pos = GetFirstViewPosition();
        MessageBox(GetNextView(pos)->m_hWnd, strMsgContent, strProductName, MB_OK | MB_ICONINFORMATION);
    }

    return TRUE;
}

void TaskDocument::SetModifyStatus()
{
    BOOL bModified = m_pTaskItem->IsModified();
    SetModifiedFlag(bModified);
}

END_CUTLEADER_NAMESPACE();
