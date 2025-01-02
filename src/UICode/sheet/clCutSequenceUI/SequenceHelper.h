#pragma once

#include "clCutSequenceUICommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(ConfigData)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(SequenceConfigItem)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequenceUI_Export SequenceHelper
{
public:
	// 该函数为选中的轮廓创建飞切工序。
	// 注：
	// 1) 传入的轮廓实例集合中不能没有元素。
	// 2) 该函数比较胖，包括可行性检查，执行命令创建工序，等等。如果无法创建飞切，该函数内有警告消息。
	static bool CreateFlyCutSequence(const CWnd* pView, const CString& strEditorName, LoopInstanceListPtr pSelectedLoopInsts,
		PartInstanceListPtr pPartInstances, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData);

	// 得到实际切割零件的工序。
	// 注：
	// 1) 这些工序是要加工的，比如轮廓工序对应的轮廓不能标记为“不加工”。
	static CutSequenceListPtr GetCutPartSequences(const CutSequenceList* pAllCutSequences);
};

END_CUTLEADER_NAMESPACE()
