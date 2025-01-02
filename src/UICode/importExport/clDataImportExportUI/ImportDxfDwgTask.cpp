#include "StdAfx.h"
#include "ImportDxfDwgTask.h"

#include "ProgressData.h"
#include "FolderNode.h"
#include "DataCenterItemList.h"
#include "PathHelper.h"

#include "CLOptionInfo.h"
#include "ImpExpConfig.h"

#include "LoopProcessor.h"
#include "PartCadData.h"
#include "PatternLoopList.h"
#include "DxfDwgConvertor.h"
#include "PartBuilder.h"
#include "Loop2PartDlg.h"
#include "PartManager.h"
#include "PartsFromAsmDxf.h"
#include "PartList.h"
#include "ImportManager.h"
#include "ClDataImportExportResource.h"
#include "DxfDwgConvertor.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

ImportDxfDwgTask::ImportDxfDwgTask(FolderNodePtr pParentFolder, const std::vector<CString>& dxfDwgPathList, LONGLONG iParamConfigID)
{
	m_pParentFolder = pParentFolder;
	m_dxfDwgPathList = dxfDwgPathList;
    m_iParamConfigID = iParamConfigID;

	m_pNewDataCenterItems.reset(new DataCenterItemList);
}

ImportDxfDwgTask::~ImportDxfDwgTask(void)
{
}

void ImportDxfDwgTask::Run() 
{
	unsigned int iImportCount = m_dxfDwgPathList.size();
	double dDelta = 100.0 / iImportCount;

	CString str;
	for (unsigned int i = 0; i < iImportCount; i++)
	{
		CString strDxfDwgPath = m_dxfDwgPathList[i];

        // ��dxf/dwg�ļ�����Ϊ�����������ָ��������Ŀ¼�С�
        {
            // ����dxf/dwg�ļ�ΪPartCadData���󣬴�ʱ��Ҫ�������л�·�������Ӱ�����˹�ϵʶ��
            BOOL bIsDwg = (PathHelper::GetFileExt(strDxfDwgPath).CompareNoCase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DWG_EXT)) == 0 ? TRUE : FALSE);
            PartCadDataPtr pPartCadData = DxfDwgConvertor::DxfDwg2CadData(strDxfDwgPath, bIsDwg);

            // ������������˹�ϵ�����з��������������
            if (!LoopProcessor::NeedToSeperate(LoopProcessor::CalcLoopTopStruct(pPartCadData->GetPatternLoopList().get()).get()))
            {
				UINT iAssignLimit = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetAssignLimit();
                PartPtr pPart = PartBuilder::BuildPart(PathHelper::GetFileName(strDxfDwgPath), pPartCadData, m_iParamConfigID, iAssignLimit, TRUE);
                DataItemPtr pNewDataItem = PartManager::SavePart(pPart, m_pParentFolder);
                m_pNewDataCenterItems->push_back(pNewDataItem);
            }
            else
            {
                // ����Ի������ڴӻ�·ʶ�������
				Loop2PartDlg loop2PartDlg;
				loop2PartDlg.Init(pPartCadData, strDxfDwgPath, m_iParamConfigID);
				if (loop2PartDlg.DoModal() == IDC_BTN_ACCEPT)
				{
					PartsFromAsmDxfPtr pPartsFromAsmDxf = loop2PartDlg.Get_PartsFromAsmDxf();
					for (unsigned int i = 0; i < pPartsFromAsmDxf->GetPartList()->size(); i++)
					{
						PartPtr pPart = pPartsFromAsmDxf->GetPartList()->operator [](i);
						DataItemPtr pNewDataItem = PartManager::SavePart(pPart, m_pParentFolder);
						m_pNewDataCenterItems->push_back(pNewDataItem);
					}
				}
            }
        }

		// ���½���
		{
			WaitForSingleObject(m_pProgressData->GetProgressDataMtx(), INFINITE);
			m_pProgressData->SetTaskPercent(m_pProgressData->GetTaskPercent() + dDelta);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DxfDwg_Imported);
            str.Replace(_T("WILLBEREPLACED1"), strDxfDwgPath);
			m_pProgressData->SetSubTask(str);
			PostMessage(m_hProgressWnd, WM_UPDATE_PROGRESS, 0, 0);
			ReleaseMutex(m_pProgressData->GetProgressDataMtx());
		}
	}

	Sleep(1000);

	// update the progress data.
	WaitForSingleObject(m_pProgressData->GetProgressDataMtx(), INFINITE);
    m_pProgressData->SetTaskPercent(100);
	m_pProgressData->IsFinished(TRUE);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_IMPORT_TASK_OK);
	m_pProgressData->SetSubTask(str);
	PostMessage(m_hProgressWnd, WM_UPDATE_PROGRESS, 0, 0);
	ReleaseMutex(m_pProgressData->GetProgressDataMtx());
}

END_CUTLEADER_NAMESPACE()
