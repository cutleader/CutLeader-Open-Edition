#pragma once

#include "clFrameCommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(ForegroundCamConnector)
DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(Sheet)

BEGIN_CUTLEADER_NAMESPACE()

// ǰ��̨camͨѶ����Ϣ���͡�
enum TwoCamMessageType
{
	// ������̨cam����Ϣ��ѯ��ͨѶ�ǲ���������
	AskBackgroundCamWhetherAlive					= 1,
	// ����ǰ̨cam����Ϣ��������ͨѶ������
	TellForegroundCamIamAlive						= 2,

	// ������̨cam����Ϣ�������򿪸�dxf���б༭��
	TellBackgroundCamToEditDxfFile					= 11,
	// ������̨cam����Ϣ��������ָ����Ľ��б༭��
	TellBackgroundCamToEditSheet					= 12,

	// ����ǰ̨cam����Ϣ����������̨cam����˱༭��
	TellForegroundCam_EditFinished					= 21,
	// ����ǰ̨cam����Ϣ����������̨camȡ���˱༭��
	TellForegroundCam_EditCanceled					= 22,

	// ��Ч��Ϣ��ǰ̨cam�ͺ�̨cam���Բ�����������Ϣ����ǰ̨cam���̨cam�յ��Է�����Ϣ��������û��Ҫ�ظ��Է�ʱ��
	// ��������������͵���Ϣ������Ҳ���Է�ֹĳһ���ظ�����֮ǰ����Ϣ��
	NoMessage										= 100,
};

// �����ڴ����ݣ�ǰ��̨cam����
struct TwoCamShareData
{
	// ��Ϣ���͡�
	int iMessageType;
};

// ����ฺ���ǰ̨cam��ͨѶ��
class CLFRAME_Export ForegroundCamConnector
{
public:
	ForegroundCamConnector(void);
	~ForegroundCamConnector(void);

public:
	static ForegroundCamConnectorPtr GetInstance();

public:
	// ���ڼ���̣߳���app�ฺ����á�
	void StartWatchThread();
	void EndWatchThread();

	// ����˱༭��
	void EditFinished();

	// ȡ���˱༭��
	void EditCanceled();

private:
	static unsigned int __stdcall WatchThreadFunction(void* LPParam);

private:
	// ��ɱ༭ʱ�����ɸ�ǰ̨cam��Ӧ��xml�ļ���
	static void GenerateResponseXml(const CString& strResponseFilePath, const TaskItem* pCurrentTaskItem, const Sheet* pCurrentSheet, const Point2D& referencePt);

	static bool SendMessage2ForegroundCam(TwoCamMessageType emTwoCamMessageType);

private:
	// ���̶߳�ʱ�ӹ����ڴ��ж�ȡǰ̨cam��ָ�����߳��ɺ�̨cam�������ں�̨cam�ر�ʱ���Զ���������̡߳�
	HANDLE m_hWatchThread;

	// ����������������е����ݽ��з��ʿ��ơ�
	HANDLE m_hMutex;

	// app�����������־�ü���߳�ֹͣ��
	bool m_bStopThread;

	// Ӧ��ǰ̨camʱ��Ӧ������д���������ļ����������ļ�·����ǰ̨cam�����ġ�
	CString m_strResponseFilePath;
	CString m_strNcFilePath;

private:
	static ForegroundCamConnectorPtr g_pForegroundCamConnector;
};

END_CUTLEADER_NAMESPACE()
