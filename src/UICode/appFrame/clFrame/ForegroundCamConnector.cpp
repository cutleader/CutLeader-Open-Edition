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

// 前台cam进程名称。
#define ForegroundCamName				   L"NcEditor-CShape.exe"

#define TwoCamShareDataSemaphoreName	   L"TwoCamShareDataSemaphoreName"
#define SharedMemery_4_BackgroundCam       L"SharedMemery_4_BackgroundCam"

// 前台cam发给后台cam的请求文件。
#define	TwoCamShare_Request_File		   L"twoCamShare_request.xml"

// 这个变量为true时，允许后台cam先启动好，此时前台cam可能还没启动，一般用于调试。
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
	// 启动线程。
	m_hWatchThread = (HANDLE)_beginthreadex(NULL, 0, &WatchThreadFunction, this, 0, NULL);
}

void ForegroundCamConnector::EndWatchThread()
{
	// 停止线程。
	WaitForSingleObject(m_hMutex, INFINITE);
	m_bStopThread = true;
	ReleaseMutex(m_hMutex);
	if (NULL != m_hWatchThread)
	{
		WaitForSingleObject(m_hWatchThread, 5000);//等待线程结束  
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
		// 判断是不是要退出线程。
		{
			bool bStopThread = false;
			WaitForSingleObject(pCncControllerConnection->m_hMutex, INFINITE);
			bStopThread = pCncControllerConnection->m_bStopThread;
			ReleaseMutex(pCncControllerConnection->m_hMutex);
			if (bStopThread)
				break;
		}

		// 前台cam要是关闭了，那么后台cam也要立即关闭。
		if (!g_bAllowBackgroundCamStart4Debug)
		{
			CString strForegroundCamPath = ClFolderHelper::GetProductPath();
			strForegroundCamPath += ForegroundCamName;
			IntegerListPtr pProcessIDList = ProcessMgr::IsProcessRun(strForegroundCamPath);
			if (pProcessIDList->size() == 0)
			{
				// 关闭当前任务，不保存所做的修改。
				CLeaderDoc* pActiveDocument = DocManager::GetActiveDocument();
				if (pActiveDocument)
					pActiveDocument->OnCloseDocument();

				// 关闭后台cam。
				HANDLE hCurrentProcess = GetCurrentProcess();
				TerminateProcess(hCurrentProcess, 0);
			}
		}

		// 打开共享内存。
		PVOID pView = nullptr;
		HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SharedMemery_4_BackgroundCam);
		if (hMapFile)
			pView = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(TwoCamShareData)); // 将文件映射对象映射到当前程序的地址空间。

		if (pView) // 成功打开了共享内存。
		{
			HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TwoCamShareDataSemaphoreName);

			// 从共享内存中读出前台cam的指令。
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
			if (twoCamShareData.iMessageType == (int)AskBackgroundCamWhetherAlive) // 前台cam询问通讯是否正常。
			{
				// 发消息给前台cam，告诉它通讯正常。
				twoCamShareData.iMessageType = (int)TellForegroundCamIamAlive;
				WaitForSingleObject(hSemaphore, INFINITE);
				memcpy_s(pView, sizeof(TwoCamShareData), (void*)&twoCamShareData, sizeof(TwoCamShareData));
				ReleaseSemaphore(hSemaphore, 1, NULL);
			}
			else if (twoCamShareData.iMessageType == (int)TellBackgroundCamToEditDxfFile) // 前台cam让后台cam新建任务加载一个dxf进行编辑。
			{
				AfxGetApp()->GetMainWnd()->ShowWindow(SW_SHOW);

				// 读出request文件中的数据。
				CString strDxfPath;
				{
					CString strRequestXmlPath = ClFolderHelper::GetTwoCamSharePath() + TwoCamShare_Request_File;
					TiXmlDocument* pXmlDoc = new TiXmlDocument();
					pXmlDoc->LoadFile(W2A(strRequestXmlPath));
					TiXmlElement* pRootElement = pXmlDoc->FirstChildElement();
					TiXmlElement* pDxfPathNode = pRootElement->FirstChildElement("DxfFilePath");
					strDxfPath = StringUtil::UTF8ToUnicode(pDxfPathNode->GetText(), strlen(pDxfPathNode->GetText()));

					// 应答数据会写入这些文件。
					WaitForSingleObject(pCncControllerConnection->m_hMutex, INFINITE);				
					TiXmlElement* pNode_ResponseFilePath = pRootElement->FirstChildElement("ResponseFilePath");
					TiXmlElement* pNode_NcFilePath = pRootElement->FirstChildElement("NcFilePath");
					pCncControllerConnection->m_strResponseFilePath = StringUtil::UTF8ToUnicode(pNode_ResponseFilePath->GetText(), strlen(pNode_ResponseFilePath->GetText()));
					pCncControllerConnection->m_strNcFilePath = StringUtil::UTF8ToUnicode(pNode_NcFilePath->GetText(), strlen(pNode_NcFilePath->GetText()));
					ReleaseMutex(pCncControllerConnection->m_hMutex);

					delete pXmlDoc;
					DeleteFile(strRequestXmlPath); // 读完就删除request文件。
				}

				// 设置无效消息，防止重复处理。
				twoCamShareData.iMessageType = (int)NoMessage;
				WaitForSingleObject(hSemaphore, INFINITE);
				memcpy_s(pView, sizeof(TwoCamShareData), (void*)&twoCamShareData, sizeof(TwoCamShareData));
				ReleaseSemaphore(hSemaphore, 1, NULL);

				// 给主线程发送消息，让它新建任务加载dxf进行编辑。
				CString* pString = new CString(strDxfPath);
				AfxGetApp()->PostThreadMessage(WM_NewTaskAndAddDxf2Canvas, (WPARAM)pString, NULL);
			}
			else if (twoCamShareData.iMessageType == (int)TellBackgroundCamToEditSheet) // 前台cam让后台cam打开一个sheet进行编辑。
			{
				AfxGetApp()->GetMainWnd()->ShowWindow(SW_SHOW);

				// 读出任务和板材信息。
				CString strRequestXmlPath = ClFolderHelper::GetTwoCamSharePath() + TwoCamShare_Request_File;
				TiXmlDocument* pXmlDoc = new TiXmlDocument();
				pXmlDoc->LoadFile(W2A(strRequestXmlPath));
				TiXmlElement* pRootElement = pXmlDoc->FirstChildElement();
				TiXmlElement* pTaskIDNode = pRootElement->FirstChildElement("TaskID");
				TiXmlElement* pSheetIDNode = pRootElement->FirstChildElement("SheetID");
				long long iTaskID = atoll(pTaskIDNode->GetText());
				long long iSheetID = atoll(pSheetIDNode->GetText());
				DeleteFile(strRequestXmlPath); // 读完就删除request文件。

				// 设置无效消息，防止重复处理。
				twoCamShareData.iMessageType = (int)NoMessage;
				WaitForSingleObject(hSemaphore, INFINITE);
				memcpy_s(pView, sizeof(TwoCamShareData), (void*)&twoCamShareData, sizeof(TwoCamShareData));
				ReleaseSemaphore(hSemaphore, 1, NULL);

				// 给主线程发送消息，让它新建任务加载dxf进行编辑。
				pair<long long, long long>* pTaskAndSheetID = new pair<long long, long long>(make_pair(iTaskID, iSheetID));
				AfxGetApp()->PostThreadMessage(WM_OpenSpecifiedSheet, (WPARAM)pTaskAndSheetID, NULL);
			}

			CloseHandle(hSemaphore);
		}

		// 清理共享内存句柄。
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

	// 打开共享内存。
	PVOID pView = nullptr;
	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SharedMemery_4_BackgroundCam);
	if (hMapFile)
		pView = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(TwoCamShareData)); // 将文件映射对象映射到当前程序的地址空间。

	if (pView) // 成功打开了共享内存。
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

	// 清理共享内存句柄。
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

	// 根节点。
	TiXmlElement* pRootNode = new TiXmlElement("Response");
	pXmlDoc->LinkEndChild(pRootNode);

	// 添加任务节点。
	{
		TiXmlElement* pTaskNode = new TiXmlElement("TaskID");
		char tmpStr[100];
		sprintf_s(tmpStr, "%I64d", pCurrentTaskItem->GetID());
		TiXmlText* pContent = new TiXmlText(tmpStr);
		pTaskNode->LinkEndChild(pContent);
		pRootNode->LinkEndChild(pTaskNode);
	}

	// 添加板材节点。
	{
		TiXmlElement* pSheetNode = new TiXmlElement("SheetID");
		char tmpStr[100];
		sprintf_s(tmpStr, "%I64d", pCurrentSheet->GetID());
		TiXmlText* pContent = new TiXmlText(tmpStr);
		pSheetNode->LinkEndChild(pContent);
		pRootNode->LinkEndChild(pSheetNode);
	}

	// 添加参考点节点。
	{
		TiXmlElement* pReferencePtNode = new TiXmlElement("ReferencePt");
		pReferencePtNode->SetDoubleAttribute("ReferencePtX", referencePt.X());
		pReferencePtNode->SetDoubleAttribute("ReferencePtY", referencePt.Y());
		pRootNode->LinkEndChild(pReferencePtNode);
	}

	// 添加板材的几何数据。
	{
		TiXmlElement* pSheetGeometryNode = new TiXmlElement("SheetGeometry");
		pRootNode->LinkEndChild(pSheetGeometryNode);

		// 写入每个轮廓实例的几何数据。
		LoopInstanceListPtr pAllLoopInstances = LoopInstanceManager::GetLoopInstanceList(pCurrentSheet->GetPartInstances());
		for (unsigned int i = 0; i < pAllLoopInstances->size(); i++)
		{
			const LoopInstance* pLoopInstance = pAllLoopInstances->operator[](i).get();

			TiXmlElement* pLoopInstanceGeometryNode = new TiXmlElement("LoopInstanceGeometry");
			pSheetGeometryNode->LinkEndChild(pLoopInstanceGeometryNode);

			// 记录轮廓实例外接矩形。
			{
				const Rect2D& outerRect = pLoopInstance->GetCacheData()->GetOuterRect();
				pLoopInstanceGeometryNode->SetDoubleAttribute("RectMinX", outerRect.GetXMin());
				pLoopInstanceGeometryNode->SetDoubleAttribute("RectMaxX", outerRect.GetXMax());
				pLoopInstanceGeometryNode->SetDoubleAttribute("RectMinY", outerRect.GetYMin());
				pLoopInstanceGeometryNode->SetDoubleAttribute("RectMaxY", outerRect.GetYMax());
			}

			// 记录轮廓实例的二维直线和圆弧数据。
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

	// 得到当前板材。
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

	// 检查当前板材上面的工序。
	{
		// 如果板材中没有工序，需要提示用户加工序后才能完成编辑。
		if (pSheet->GetCutSequences()->size() == 0) {
			CString strMessage = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Return2ForegroundCam_NoSequence);
			::MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, strMessage, strProductName, MB_OK | MB_ICONWARNING);
			return;
		}

		PartInstanceListPtr pPartInstances = pSheet->GetPartInstances();
		PartInstanceListPtr pUnCutPartInstances = pSheet->GetCutSequences()->FilterUnSeqPartInst(pPartInstances);
		if (pUnCutPartInstances->size() > 0) { // 尚有零件没有加完工序。
			CString strMessage = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Return2ForegroundCam_SomeLoopNoSequence);
			if (::MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, strMessage, strProductName, MB_YESNO | MB_ICONQUESTION) != IDYES)
				return;
		}
	}

	// 保存当前的任务对象。
	if (pTaskItem->IsModified()) {
		DataProperty taskProp;
		DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
		if (!DataCenterItemLoader::GetDataProp(pMainDBCon, pTaskItem->GetID(), taskProp)) { // 数据库中没有该任务。
			FolderNodePtr pParentFolder = DataCenterItemLoader::LoadProductTree(pMainDBCon);
			pTaskItem->SetCreateTime(DateHelper::GetCurrentTime());
			pTaskItem->SetModifyTime(DateHelper::GetCurrentTime());
			TaskItemManager::SaveNewTaskItem(pTaskItem, pParentFolder);
		}
		else // 数据库中已经有了该任务。
			TaskItemManager::SaveExistTaskItem(pTaskItem);
	}

	// 检查应答文件的路径是否合法。
	CString strResponseFilePath, strNcFilePath;
	{
		WaitForSingleObject(m_hMutex, INFINITE);
		strResponseFilePath = m_strResponseFilePath;
		strNcFilePath = m_strNcFilePath;
		ReleaseMutex(m_hMutex);

		// 应答文件不能已经存在了。
		CFileFind fileFind;
		if (fileFind.FindFile(strResponseFilePath) || fileFind.FindFile(strNcFilePath)) {
			::MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, _T("应答文件已经存在，请确保应答文件不存在。"), strProductName, MB_OK | MB_ICONWARNING);
			return;
		}
		fileFind.Close();

		// 测试是不是可以写应答文件。
		{
			HANDLE hFile = CreateFile(strResponseFilePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE) {
				::MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, _T("无法写应答文件，请确保应答文件的路径是否正确。"), strProductName, MB_OK | MB_ICONWARNING);
				return;
			}
			CloseHandle(hFile);
		}
		{
			HANDLE hFile = CreateFile(strNcFilePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE) {
				::MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, _T("无法写应答文件，请确保应答文件的路径是否正确。"), strProductName, MB_OK | MB_ICONWARNING);
				return;
			}
			CloseHandle(hFile);
		}
	}

	// 得到参考点。
	IMaterialSizePtr pMaterialSize = MaterialLibLoader::LoadMatSize(DataBaseManager::GetInstance()->GetMatLibDBConnect(), pSheet->GetMaterialSizeID());
	bool bCannotFindRefLoopsOnCanvas = false;
	Point2D referencePt = CutSequenceManager::CalcReferencePt(pSheet->GetPartPlacements().get(), pMaterialSize.get(), pSheet->GetRefPtData().get(), bCannotFindRefLoopsOnCanvas);

	// 生成g代码，并存到指定目录下。
	SheetHelper::CreateNcAndSave(pSheet, strNcFilePath, referencePt);

	// 生成应答xml文件。
	GenerateResponseXml(strResponseFilePath, pTaskItem.get(), pSheet.get(), referencePt);

	// 关闭当前任务。
	DocManager::GetActiveDocument()->OnCloseDocument();

	// 隐藏后台cam。
	AfxGetApp()->GetMainWnd()->ShowWindow(SW_HIDE);

	// 通知前台cam。
	SendMessage2ForegroundCam(TellForegroundCam_EditFinished);
}

void ForegroundCamConnector::EditCanceled()
{
	// 关闭当前任务，不保存所做的修改。
	DocManager::GetActiveDocument()->OnCloseDocument();

	// 隐藏后台cam。
	AfxGetApp()->GetMainWnd()->ShowWindow(SW_HIDE);

	// 通知前台cam。
	ForegroundCamConnector::GetInstance()->SendMessage2ForegroundCam(TellForegroundCam_EditCanceled);
}

END_CUTLEADER_NAMESPACE()
