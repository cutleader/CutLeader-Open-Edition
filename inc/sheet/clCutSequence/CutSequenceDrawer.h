#pragma once

#include "clCutSequenceCommon.h"
#include "BasicGeometryDrawer.h"

DECLARE_CUTLEADER_CLASS(ICutSequence)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(SequenceSimulateShapeList)
DECLARE_CUTLEADER_CLASS(IChainNode)
DECLARE_CUTLEADER_CLASS(CutNodeList)

BEGIN_CUTLEADER_NAMESPACE()

// “工序绘制器”。
// 注：
//   1) 绘制工序其实就是绘制切割路径。涉及到轮廓部分绘制时，不使用工序颜色，而是保持轮廓原先颜色。
//   2) 绘制时不考虑偏移，因为这毕竟不是切割模拟。
class ClCutSequence_Export CutSequenceDrawer : public BasicGeometryDrawer
{
public:
	CutSequenceDrawer(GlViewPortPtr pViewPort);
	~CutSequenceDrawer(void);

public:
	// 绘制当前的工序。
	/* 参数：
	*  pCutNodeList: 所有切割节点。
	*/
	void DrawActiveCutSequence(const ICutSequence* pCutSequence, const CutNodeList* pCutNodeList);
	void DrawActiveCutSequences(const CutSequenceList* pCutSequences, const CutNodeList* pCutNodeList);

	// 绘制选中的工序。
	/* 参数：
	*  pCutNodeList: 所有切割节点。
	*/
	void DrawSelectedCutSequence(const ICutSequence* pCutSequence, const CutNodeList* pCutNodeList);
	void DrawSelectedCutSequences(const CutSequenceList* pCutSequences, const CutNodeList* pCutNodeList);

	// 绘制“模拟形状”。
	// 注：
	//  1) 快速移动线的线型在这个函数中定为虚线，其他绘图样式由调用者负责设置。
    //  2) bIgnoreCutPathOnLoop: 是否不画轮廓上的切割节点，这样可以避免把轮廓上的刀具颜色覆盖掉。
	void DrawSequenceSimulateShapes(const SequenceSimulateShapeList* pSequenceSimulateShapes, bool bIgnoreCutPathOnLoop = true);

	// 绘制工序。
	// 注：
	//  1) 函数中负责绘图样式的设置。
	void DrawCutSequence(const ICutSequence* pCutSequence, const CutNodeList* pCutNodeList);
	void DrawCutSequences(const CutSequenceList* pCutSequences, const CutNodeList* pCutNodeList);

	// 绘制“连割节点”。
	// 注：
	//  1) “连割点”节点用十字叉表示，对于“连割回路”节点就绘制轮廓。
    //  2) 这些函数负责设置绘图样式，无论高亮还是选中，不改变轮廓颜色，不过其他类型的连割节点就用系统的高亮和选中色。
    void DrawActiveChainNode(const IChainNode* pChainNode);
    void DrawSelectedChainNode(const IChainNode* pChainNode);

private:
    // 沿着一些模拟图形的两侧绘制箭头。
    // 注：
    //  1）一般画在快速移动线，余料切割线，桥接工序中的桥的两侧，等等。
    void DrawArrow(const ILineArc2D* pLineArc2D);
};

END_CUTLEADER_NAMESPACE()
