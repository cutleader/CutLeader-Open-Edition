#pragma once

#include "ClTaskCommon.h"
#include "RptPageBuilderBase.h"

DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(ColumnData)
DECLARE_CUTLEADER_CLASS(TaskItem)

BEGIN_CUTLEADER_NAMESPACE()

// 对应“任务模板页”的报表生成器。
class TaskPageBuilder : public RptPageBuilderBase
{
public:
	TaskPageBuilder(TaskItemPtr pTaskItem);
	~TaskPageBuilder(void);

public: // implement PageDataParser interface.
	virtual CString GetFieldValue(BIND_ITEM_TYPE bindDataType);
	virtual ImgDataPtr GetImageValue(int Width, int iHeight, BIND_ITEM_TYPE bindDataType);
	virtual ColumnDataPtr GetTableColData(int iColumnIndex, BIND_ITEM_TYPE tableBindItem, BIND_ITEM_TYPE colBindItem);

private:
	// 得到几种时间，单位是秒。要是参数没有设置，返回0
	int GetTotalCutTime(); // 得到总切割时间
	int GetTotalRapidTime(); // 得到总快速移动时间
	double GetTotalPierceTime(); // 得到总刺穿时间，这里返回浮点数

    // 任务的材料利用率。
    double CalcMatUtilization();

private:
	TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE()
