#pragma once

#include "ClTaskCommon.h"
#include "RptPageBuilderBase.h"

DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(ColumnData)
DECLARE_CUTLEADER_CLASS(TaskItem)

BEGIN_CUTLEADER_NAMESPACE()

// ��Ӧ������ģ��ҳ���ı�����������
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
	// �õ�����ʱ�䣬��λ���롣Ҫ�ǲ���û�����ã�����0
	int GetTotalCutTime(); // �õ����и�ʱ��
	int GetTotalRapidTime(); // �õ��ܿ����ƶ�ʱ��
	double GetTotalPierceTime(); // �õ��̴ܴ�ʱ�䣬���ﷵ�ظ�����

    // ����Ĳ��������ʡ�
    double CalcMatUtilization();

private:
	TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE()
