#include "StdAfx.h"
#include "ForegroundCamConnector.h"

#include "LogMgr.h"
#include "tinyxml.h"
#include "ClFolderHelper.h"
#include "StringUtil.h"
#include "ClWindowFinder.h"
#include "DataBaseManager.h"
#include "DateHelper.h"
#include "DataCenterItemLoader.h"
#include "ProcessMgr.h"
#include "IntegerList.h"
#include "clBaseDataResource.h"
#include "LineArc2DList.h"
#include "Line2D.h"
#include "Arc2D.h"

#include "ParamConfigLoader.h"
#include "MachineItemLoader.h"
#include "MachineParam.h"
#include "ExpertLibItemLoader.h"
#include "MachineLibHelper.h"
#include "SequenceConfigItem.h"
#include "ParamConfig.h"
#include "MaterialLibLoader.h"

#include "TaskItem.h"
#include "Sheet.h"
#include "SheetList.h"
#include "TaskView.h"
#include "SheetSubView.h"
#include "clFrameUIResource.h"
#include "SheetHelper.h"
#include "DocManager.h"
#include "MaterialStockWarningDlg.h"
#include "TaskItemManager.h"
#include "TaskHelper.h"
#include "CutSequenceList.h"
#include "PartInstanceList.h"
#include "LoopInstanceManager.h"
#include "LoopInstance.h"
#include "LoopInstanceList.h"
#include "LoopInstanceCache.h"
#include "IPatternLoop.h"
#include "PatternLoopCache.h"
#include "CutSequenceManager.h"

// ǰ̨cam�������ơ�
#define ForegroundCamName				   L"NcEditor-CShape.exe"

#define TwoCamShareDataSemaphoreName	   L"TwoCamShareDataSemaphoreName"
#define SharedMemery_4_BackgroundCam       L"SharedMemery_4_BackgroundCam"

// ǰ̨cam������̨cam�������ļ���
#define	TwoCamShare_Request_File		   L"twoCamShare_request.xml"

// �������Ϊtrueʱ�������̨cam�������ã���ʱǰ̨cam���ܻ�û������һ�����ڵ��ԡ�
bool g_bAllowBackgroundCamStart4Debug = true;

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

ForegroundCamConnectorPtr ForegroundCamConnector::g_pForegroundCamConnector;

ForegroundCamConnector::ForegroundCamConnector(void)
{
	m_hWatchThread = nullptr;
	m_hMutex = nullptr;
	m_bStopThread = false;
}

ForegroundCamConnector::~ForegroundCamConnector(void)
{
}

ForegroundCamConnectorPtr ForegroundCamConnector::GetInstance()
{
	if (g_pForegroundCamConnector.get() == NULL)
		g_pForegroundCamConnector.reset(new ForegroundCamConnector());
	return g_pForegroundCamConnector;
}

void ForegroundCamConnector::StartWatchThread()
{
	// �����̡߳�
	m_hWatchThread = (HANDLE)_beginthreadex(NULL, 0, &WatchThreadFunction, this, 0, NULL);
}

void ForegroundCamConnector::EndWatchThread()
{
	// ֹͣ�̡߳�
	WaitForSingleObject(m_hMutex, INFINITE);
	m_bStopThread = true;
	ReleaseMutex(m_hMutex);
	if (NULL != m_hWatchThread)
	{
		WaitForSingleObject(m_hWatchThread, 5000);//�ȴ��߳̽���  
		CloseHandle(m_hWatchThread);
		m_hWatchThread = NULL;
	}
}

unsigned int __stdcall ForegroundCamConnector::WatchThreadFunction(void* LPParam)
{
	ForegroundCamConnector* pCncControllerConnection = static_cast<ForegroundCamConnector*>(LPParam);

	USES_CONVERSION;

	while (true)
	{
		// �ж��ǲ���Ҫ�˳��̡߳�
		{
			bool bStopThread = false;
			WaitForSingleObject(pCncControllerConnection->m_hMutex, INFINITE);
			bStopThread = pCncControllerConnection->m_bStopThread;
			ReleaseMutex(pCncControllerConnection->m_hMutex);
			if (bStopThread)
				break;
		}

		// ǰ̨camҪ�ǹر��ˣ���ô��̨camҲҪ�����رա�
		if (!g_bAllowBackgroundCamStart4Debug)
		{
			CString strForegroundCamPath = ClFolderHelper::GetProductPath();
			strForegroundCamPath += ForegroundCamName;
			IntegerListPtr pProcessIDList = ProcessMgr::IsProcessRun(strForegroundCamPath);
			if (pProcessIDList->size() == 0)
			{
				// �رյ�ǰ���񣬲������������޸ġ�
				CLeaderDoc* pActiveDocument = DocManager::GetActiveDocument();
				if (pActiveDocument)
					pActiveDocument->OnCloseDocument();

				// �رպ�̨cam��
				HANDLE hCurrentProcess = GetCurrentProcess();
				TerminateProcess(hCurrentProcess, 0);
			}
		}

		// �򿪹����ڴ档
		PVOID pView = nullptr;
		HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SharedMemery_4_BackgroundCam);
		if (hMapFile)
			pView = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(TwoCamShareData)); // ���ļ�ӳ�����ӳ�䵽��ǰ����ĵ�ַ�ռ䡣

		if (pView) // �ɹ����˹����ڴ档
		{
			HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TwoCamShareDataSemaphoreName);

			// �ӹ����ڴ��ж���ǰ̨cam��ָ�
			TwoCamShareData twoCamShareData;
			{
				WaitForSingleObject(hSemaphore, INFINITE);
				memcpy_s((void *)&twoCamShareData, sizeof(twoCamShareData), pView, sizeof(TwoCamShareData));
				ReleaseSemaphore(hSemaphore, 1, NULL);
			}

			if (twoCamShareData.iMessageType == (int)NoMessage)
			{
				;
			}
			if (twoCamShareData.iMessageType == (int)AskBackgroundCamWhetherAlive) // ǰ̨camѯ��ͨѶ�Ƿ�������
			{
				// ����Ϣ��ǰ̨cam��������ͨѶ������
				twoCamShareData.iMessageType = (int)TellForegroundCamIamAlive;
				WaitForSingleObject(hSemaphore, INFINITE);
				memcpy_s(pView, sizeof(TwoCamShareData), (void*)&twoCamShareData, sizeof(TwoCamShareData));
				ReleaseSemaphore(hSemaphore, 1, NULL);
			}
			else if (twoCamShareData.iMessageType == (int)TellBackgroundCamToEditDxfFile) // ǰ̨cam�ú�̨cam�½��������һ��dxf���б༭��
			{
				AfxGetApp()->GetMainWnd()->ShowWindow(SW_SHOW);

				// ����request�ļ��е����ݡ�
				CString strDxfPath;
				{
					CString strRequestXmlPath = ClFolderHelper::GetTwoCamSharePath() + TwoCamShare_Request_File;
					TiXmlDocument* pXmlDoc = new TiXmlDocument();
					pXmlDoc->LoadFile(W2A(strRequestXmlPath));
					TiXmlElement* pRootElement = pXmlDoc->FirstChildElement();
					TiXmlElement* pDxfPathNode = pRootElement->FirstChildElement("DxfFilePath");
					strDxfPath = StringUtil::UTF8ToUnicode(pDxfPathNode->GetText(), strlen(pDxfPathNode->GetText()));

					// Ӧ�����ݻ�д����Щ�ļ���
					WaitForSingleObject(pCncControllerConnection->m_hMutex, INFINITE);				
					TiXmlElement* pNode_ResponseFilePath = pRootElement->FirstChildElement("ResponseFilePath");
					TiXmlElement* pNode_NcFilePath = pRootElement->FirstChildElement("NcFilePath");
					pCncControllerConnection->m_strResponseFilePath = StringUtil::UTF8ToUnicode(pNode_ResponseFilePath->GetText(), strlen(pNode_ResponseFilePath->GetText()));
					pCncControllerConnection->m_strNcFilePath = StringUtil::UTF8ToUnicode(pNode_NcFilePath->GetText(), strlen(pNode_NcFilePath->GetText()));
					ReleaseMutex(pCncControllerConnection->m_hMutex);

					delete pXmlDoc;
					DeleteFile(strRequestXmlPath); // �����ɾ��request�ļ���
				}

				// ������Ч��Ϣ����ֹ�ظ�����
				twoCamShareData.iMessageType = (int)NoMessage;
				WaitForSingleObject(hSemaphore, INFINITE);
				memcpy_s(pView, sizeof(TwoCamShareData), (void*)&twoCamShareData, sizeof(TwoCamShareData));
				ReleaseSemaphore(hSemaphore, 1, NULL);

				// �����̷߳�����Ϣ�������½��������dxf���б༭��
				CString* pString = new CString(strDxfPath);
				AfxGetApp()->PostThreadMessage(WM_NewTaskAndAddDxf2Canvas, (WPARAM)pString, NULL);
			}
			else if (twoCamShareData.iMessageType == (int)TellBackgroundCamToEditSheet) // ǰ̨cam�ú�̨cam��һ��sheet���б༭��
			{
				AfxGetApp()->GetMainWnd()->ShowWindow(SW_SHOW);

				// ��������Ͱ����Ϣ��
				CString strRequestXmlPath = ClFolderHelper::GetTwoCamSharePath() + TwoCamShare_Request_File;
				TiXmlDocument* pXmlDoc = new TiXmlDocument();
				pXmlDoc->LoadFile(W2A(strRequestXmlPath));
				TiXmlElement* pRootElement = pXmlDoc->FirstChildElement();
				TiXmlElement* pTaskIDNode = pRootElement->FirstChildElement("TaskID");
				TiXmlElement* pSheetIDNode = pRootElement->FirstChildElement("SheetID");
				long long iTaskID = atoll(pTaskIDNode->GetText());
				long long iSheetID = atoll(pSheetIDNode->GetText());
				DeleteFile(strRequestXmlPath); // �����ɾ��request�ļ���

				// ������Ч��Ϣ����ֹ�ظ�����
				twoCamShareData.iMessageType = (int)NoMessage;
				WaitForSingleObject(hSemaphore, INFINITE);
				memcpy_s(pView, sizeof(TwoCamShareData), (void*)&twoCamShareData, sizeof(TwoCamShareData));
				ReleaseSemaphore(hSemaphore, 1, NULL);

				// �����̷߳�����Ϣ�������½��������dxf���б༭��
				pair<long long, long long>* pTaskAndSheetID = new pair<long long, long long>(make_pair(iTaskID, iSheetID));
				AfxGetApp()->PostThreadMessage(WM_OpenSpecifiedSheet, (WPARAM)pTaskAndSheetID, NULL);
			}

			CloseHandle(hSemaphore);
		}

		// �������ڴ�����
		if (hMapFile)
		{
			if (pView)
			{
				UnmapViewOfFile(pView);
				pView = nullptr;
			}
			CloseHandle(hMapFile);
			hMapFile = nullptr;
		}

		Sleep(100);
	}

	return 0;
}

bool ForegroundCamConnector::SendMessage2ForegroundCam(TwoCamMessageType emTwoCamMessageType)
{
	bool bRet = false;

	// �򿪹����ڴ档
	PVOID pView = nullptr;
	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SharedMemery_4_BackgroundCam);
	if (hMapFile)
		pView = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(TwoCamShareData)); // ���ļ�ӳ�����ӳ�䵽��ǰ����ĵ�ַ�ռ䡣

	if (pView) // �ɹ����˹����ڴ档
	{
		HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TwoCamShareDataSemaphoreName);
		if (hSemaphore)
		{
			TwoCamShareData twoCamShareData;
			twoCamShareData.iMessageType = (int)emTwoCamMessageType;
			WaitForSingleObject(hSemaphore, INFINITE);
			memcpy_s(pView, sizeof(TwoCamShareData), (void*)&twoCamShareData, sizeof(TwoCamShareData));
			ReleaseSemaphore(hSemaphore, 1, NULL);
			bRet = true;

			CloseHandle(hSemaphore);
		}
	}

	// �������ڴ�����
	if (hMapFile)
	{
		if (pView)
		{
			UnmapViewOfFile(pView);
			pView = nullptr;
		}
		CloseHandle(hMapFile);
		hMapFile = nullptr;
	}

	return bRet;
}

void ForegroundCamConnector::GenerateResponseXml(const CString& strResponseFilePath, const TaskItem* pCurrentTaskItem, const Sheet* pCurrentSheet, const Point2D& referencePt)
{
	USES_CONVERSION;

	TiXmlDocument* pXmlDoc = new TiXmlDocument();

	// ���ڵ㡣
	TiXmlElement* pRootNode = new TiXmlElement("Response");
	pXmlDoc->LinkEndChild(pRootNode);

	// �������ڵ㡣
	{
		TiXmlElement* pTaskNode = new TiXmlElement("TaskID");
		char tmpStr[100];
		sprintf_s(tmpStr, "%I64d", pCurrentTaskItem->GetID());
		TiXmlText* pContent = new TiXmlText(tmpStr);
		pTaskNode->LinkEndChild(pContent);
		pRootNode->LinkEndChild(pTaskNode);
	}

	// ��Ӱ�Ľڵ㡣
	{
		TiXmlElement* pSheetNode = new TiXmlElement("SheetID");
		char tmpStr[100];
		sprintf_s(tmpStr, "%I64d", pCurrentSheet->GetID());
		TiXmlText* pContent = new TiXmlText(tmpStr);
		pSheetNode->LinkEndChild(pContent);
		pRootNode->LinkEndChild(pSheetNode);
	}

	// ��Ӳο���ڵ㡣
	{
		TiXmlElement* pReferencePtNode = new TiXmlElement("ReferencePt");
		pReferencePtNode->SetDoubleAttribute("ReferencePtX", referencePt.X());
		pReferencePtNode->SetDoubleAttribute("ReferencePtY", referencePt.Y());
		pRootNode->LinkEndChild(pReferencePtNode);
	}

	// ��Ӱ�ĵļ������ݡ�
	{
		TiXmlElement* pSheetGeometryNode = new TiXmlElement("SheetGeometry");
		pRootNode->LinkEndChild(pSheetGeometryNode);

		// д��ÿ������ʵ���ļ������ݡ�
		LoopInstanceListPtr pAllLoopInstances = LoopInstanceManager::GetLoopInstanceList(pCurrentSheet->GetPartInstances());
		for (unsigned int i = 0; i < pAllLoopInstances->size(); i++)
		{
			const LoopInstance* pLoopInstance = pAllLoopInstances->operator[](i).get();

			TiXmlElement* pLoopInstanceGeometryNode = new TiXmlElement("LoopInstanceGeometry");
			pSheetGeometryNode->LinkEndChild(pLoopInstanceGeometryNode);

			// ��¼����ʵ����Ӿ��Ρ�
			{
				const Rect2D& outerRect = pLoopInstance->GetCacheData()->GetOuterRect();
				pLoopInstanceGeometryNode->SetDoubleAttribute("RectMinX", outerRect.GetXMin());
				pLoopInstanceGeometryNode->SetDoubleAttribute("RectMaxX", outerRect.GetXMax());
				pLoopInstanceGeometryNode->SetDoubleAttribute("RectMinY", outerRect.GetYMin());
				pLoopInstanceGeometryNode->SetDoubleAttribute("RectMaxY", outerRect.GetYMax());
			}

			// ��¼����ʵ���Ķ�άֱ�ߺ�Բ�����ݡ�
			{
				LineArc2DListPtr pLineArc2Ds = pLoopInstance->GetCacheData()->GetPatternLoop()->GetCacheData()->GetLoopBaseGeom()->Clone();
				pLineArc2Ds->Transform(pLoopInstance->GetTransformMatrix());
				for (unsigned int k = 0; k < pLineArc2Ds->size(); k++)
				{
					TiXmlElement* pGeomItemNode = new TiXmlElement("LineArc2D");
					pLoopInstanceGeometryNode->LinkEndChild(pGeomItemNode);

					char tmpStr[100];
					if (const Line2D* pLine2D = dynamic_cast<const Line2D*>(pLineArc2Ds->operator [](k).get()))
					{
						pGeomItemNode->SetAttribute("IsLine", 1);
						sprintf_s(tmpStr, "%.16f", pLine2D->GetStartPoint().X());
						pGeomItemNode->SetAttribute("StartPtX", tmpStr);
						sprintf_s(tmpStr, "%.16f", pLine2D->GetStartPoint().Y());
						pGeomItemNode->SetAttribute("StartPtY", tmpStr);
						sprintf_s(tmpStr, "%.16f", pLine2D->GetEndPoint().X());
						pGeomItemNode->SetAttribute("EndPtX", tmpStr);
						sprintf_s(tmpStr, "%.16f", pLine2D->GetEndPoint().Y());
						pGeomItemNode->SetAttribute("EndPtY", tmpStr);
					}
					else if (const Arc2D* pArcItem = dynamic_cast<const Arc2D*>(pLineArc2Ds->operator [](k).get()))
					{
						pGeomItemNode->SetAttribute("IsArc", 1);
						sprintf_s(tmpStr, "%.16f", pArcItem->GetBasePoint().X());
						pGeomItemNode->SetAttribute("CenterPtX", tmpStr);
						sprintf_s(tmpStr, "%.16f", pArcItem->GetBasePoint().Y());
						pGeomItemNode->SetAttribute("CenterPtY", tmpStr);
						sprintf_s(tmpStr, "%.16f", pArcItem->GetRadius());
						pGeomItemNode->SetAttribute("Radius", tmpStr);
						sprintf_s(tmpStr, "%.16f", pArcItem->GetStartAngle());
						pGeomItemNode->SetAttribute("StartAngle", tmpStr);
						sprintf_s(tmpStr, "%.16f", pArcItem->GetSweepAngle());
						pGeomItemNode->SetAttribute("SweepAngle", tmpStr);
					}
				}
			}
		}
	}

	pXmlDoc->SaveFile(W2A(strResponseFilePath));
}

void ForegroundCamConnector::EditFinished()
{
	USES_CONVERSION;

	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// �õ���ǰ��ġ�
	TaskItemPtr pTaskItem;
	SheetPtr pSheet;
	{
		TaskView* pTaskView = (TaskView*)ClWindowFinder::GetCurrentView_2();
		if (pTaskView)
			pTaskItem = pTaskView->GetTaskItem();
		else {
			LogMgr::DebugWarn(_T("653981"));
			return;
		}

		SheetSubView* pSheetSubView = (SheetSubView*)ClWindowFinder::GetCurrentView_1();
		if (pSheetSubView)
			pSheet = pSheetSubView->GetSheet();
		else {
			LogMgr::DebugWarn(_T("986732"));
			return;
		}
	}

	// ��鵱ǰ�������Ĺ���
	{
		// ��������û�й�����Ҫ��ʾ�û��ӹ���������ɱ༭��
		if (pSheet->GetCutSequences()->size() == 0) {
			CString strMessage = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Return2ForegroundCam_NoSequence);
			::MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, strMessage, strProductName, MB_OK | MB_ICONWARNING);
			return;
		}

		PartInstanceListPtr pPartInstances = pSheet->GetPartInstances();
		PartInstanceListPtr pUnCutPartInstances = pSheet->GetCutSequences()->FilterUnSeqPartInst(pPartInstances);
		if (pUnCutPartInstances->size() > 0) { // �������û�м��깤��
			CString strMessage = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Return2ForegroundCam_SomeLoopNoSequence);
			if (::MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, strMessage, strProductName, MB_YESNO | MB_ICONQUESTION) != IDYES)
				return;
		}
	}

	// ���浱ǰ���������
	if (pTaskItem->IsModified()) {
		DataProperty taskProp;
		DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
		if (!DataCenterItemLoader::GetDataProp(pMainDBCon, pTaskItem->GetID(), taskProp)) { // ���ݿ���û�и�����
			FolderNodePtr pParentFolder = DataCenterItemLoader::LoadProductTree(pMainDBCon);
			pTaskItem->SetCreateTime(DateHelper::GetCurrentTime());
			pTaskItem->SetModifyTime(DateHelper::GetCurrentTime());
			TaskItemManager::SaveNewTaskItem(pTaskItem, pParentFolder);
		}
		else // ���ݿ����Ѿ����˸�����
			TaskItemManager::SaveExistTaskItem(pTaskItem);
	}

	// ���Ӧ���ļ���·���Ƿ�Ϸ���
	CString strResponseFilePath, strNcFilePath;
	{
		WaitForSingleObject(m_hMutex, INFINITE);
		strResponseFilePath = m_strResponseFilePath;
		strNcFilePath = m_strNcFilePath;
		ReleaseMutex(m_hMutex);

		// Ӧ���ļ������Ѿ������ˡ�
		CFileFind fileFind;
		if (fileFind.FindFile(strResponseFilePath) || fileFind.FindFile(strNcFilePath)) {
			::MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, _T("Ӧ���ļ��Ѿ����ڣ���ȷ��Ӧ���ļ������ڡ�"), strProductName, MB_OK | MB_ICONWARNING);
			return;
		}
		fileFind.Close();

		// �����ǲ��ǿ���дӦ���ļ���
		{
			HANDLE hFile = CreateFile(strResponseFilePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE) {
				::MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, _T("�޷�дӦ���ļ�����ȷ��Ӧ���ļ���·���Ƿ���ȷ��"), strProductName, MB_OK | MB_ICONWARNING);
				return;
			}
			CloseHandle(hFile);
		}
		{
			HANDLE hFile = CreateFile(strNcFilePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE) {
				::MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, _T("�޷�дӦ���ļ�����ȷ��Ӧ���ļ���·���Ƿ���ȷ��"), strProductName, MB_OK | MB_ICONWARNING);
				return;
			}
			CloseHandle(hFile);
		}
	}

	// �õ��ο��㡣
	IMaterialSizePtr pMaterialSize = MaterialLibLoader::LoadMatSize(DataBaseManager::GetInstance()->GetMatLibDBConnect(), pSheet->GetMaterialSizeID());
	bool bCannotFindRefLoopsOnCanvas = false;
	Point2D referencePt = CutSequenceManager::CalcReferencePt(pSheet->GetPartPlacements().get(), pMaterialSize.get(), pSheet->GetRefPtData().get(), bCannotFindRefLoopsOnCanvas);

	// ����g���룬���浽ָ��Ŀ¼�¡�
	SheetHelper::CreateNcAndSave(pSheet, strNcFilePath, referencePt);

	// ����Ӧ��xml�ļ���
	GenerateResponseXml(strResponseFilePath, pTaskItem.get(), pSheet.get(), referencePt);

	// �رյ�ǰ����
	DocManager::GetActiveDocument()->OnCloseDocument();

	// ���غ�̨cam��
	AfxGetApp()->GetMainWnd()->ShowWindow(SW_HIDE);

	// ֪ͨǰ̨cam��
	SendMessage2ForegroundCam(TellForegroundCam_EditFinished);
}

void ForegroundCamConnector::EditCanceled()
{
	// �رյ�ǰ���񣬲������������޸ġ�
	DocManager::GetActiveDocument()->OnCloseDocument();

	// ���غ�̨cam��
	AfxGetApp()->GetMainWnd()->ShowWindow(SW_HIDE);

	// ֪ͨǰ̨cam��
	ForegroundCamConnector::GetInstance()->SendMessage2ForegroundCam(TellForegroundCam_EditCanceled);
}

END_CUTLEADER_NAMESPACE()
