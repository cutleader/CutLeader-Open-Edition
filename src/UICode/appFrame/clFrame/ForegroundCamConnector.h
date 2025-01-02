#pragma once

#include "clFrameCommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(ForegroundCamConnector)
DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(Sheet)

BEGIN_CUTLEADER_NAMESPACE()

// 前后台cam通讯的消息类型。
enum TwoCamMessageType
{
	// 发给后台cam的消息，询问通讯是不是正常。
	AskBackgroundCamWhetherAlive					= 1,
	// 发给前台cam的消息，告诉它通讯正常。
	TellForegroundCamIamAlive						= 2,

	// 发给后台cam的消息，让它打开该dxf进行编辑。
	TellBackgroundCamToEditDxfFile					= 11,
	// 发给后台cam的消息，让它打开指定板材进行编辑。
	TellBackgroundCamToEditSheet					= 12,

	// 发给前台cam的消息，告诉它后台cam完成了编辑。
	TellForegroundCam_EditFinished					= 21,
	// 发给前台cam的消息，告诉它后台cam取消了编辑。
	TellForegroundCam_EditCanceled					= 22,

	// 无效消息，前台cam和后台cam可以不必理睬这个消息。当前台cam或后台cam收到对方的消息，但是又没必要回复对方时，
	// 可以设置这个类型的消息，这样也可以防止某一方重复处理之前的消息。
	NoMessage										= 100,
};

// 共享内存数据，前后台cam共享。
struct TwoCamShareData
{
	// 消息类型。
	int iMessageType;
};

// 这个类负责和前台cam的通讯。
class CLFRAME_Export ForegroundCamConnector
{
public:
	ForegroundCamConnector(void);
	~ForegroundCamConnector(void);

public:
	static ForegroundCamConnectorPtr GetInstance();

public:
	// 关于监控线程，由app类负责调用。
	void StartWatchThread();
	void EndWatchThread();

	// 完成了编辑。
	void EditFinished();

	// 取消了编辑。
	void EditCanceled();

private:
	static unsigned int __stdcall WatchThreadFunction(void* LPParam);

private:
	// 完成编辑时，生成给前台cam的应答xml文件。
	static void GenerateResponseXml(const CString& strResponseFilePath, const TaskItem* pCurrentTaskItem, const Sheet* pCurrentSheet, const Point2D& referencePt);

	static bool SendMessage2ForegroundCam(TwoCamMessageType emTwoCamMessageType);

private:
	// 该线程定时从共享内存中读取前台cam的指令。这个线程由后台cam启动，在后台cam关闭时会自动结束这个线程。
	HANDLE m_hWatchThread;

	// 这个互斥体对这个类中的数据进行访问控制。
	HANDLE m_hMutex;

	// app类设置这个标志让监控线程停止。
	bool m_bStopThread;

	// 应答前台cam时，应答数据写入这两个文件，这两个文件路径是前台cam送来的。
	CString m_strResponseFilePath;
	CString m_strNcFilePath;

private:
	static ForegroundCamConnectorPtr g_pForegroundCamConnector;
};

END_CUTLEADER_NAMESPACE()
