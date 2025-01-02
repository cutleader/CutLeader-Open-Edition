#pragma once

#include "clFrameCommon.h"
#include <vector>
#include "Matrix2D.h"
#include "clGeometryFeatureCommon.h"
#include "clCutFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(TextStructList)
DECLARE_CUTLEADER_CLASS(IEditor)
DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(CutSequenceDeleteCommand)
DECLARE_CUTLEADER_CLASS(SequenceSimulateShapeList)
DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(LoopToolDataList)
DECLARE_CUTLEADER_CLASS(MacRouterEditor)
DECLARE_CUTLEADER_CLASS(PartLayoutEditor)
DECLARE_CUTLEADER_CLASS(PartItemList)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(ReportData)
DECLARE_CUTLEADER_CLASS(CommandList)
DECLARE_CUTLEADER_CLASS(CanvasGeometryEditor)
DECLARE_CUTLEADER_CLASS(SequenceEditor)

BEGIN_CUTLEADER_NAMESPACE()

// 板材视图助手。
// 注：
// 1) SheetSubView中的业务功能移到这个类中，让SheetSubView更像视图。
class SheetViewHelper
{
	friend class SheetSubView;

protected:
	/************************************************************************/
	// 有关选择集。

	// 得到选中的轮廓实例。
	// 注：
	// 1) 底图或板材模式下，当处于选择action时，返回选中的轮廓实例。
	// 2) 如果当前处于底图下选择图元的状态，会返回选中图元所在的轮廓。
	static LoopInstanceListPtr GetSelectedLoopInstances(const IEditor* pEditor);

	// 得到当前选中的几何特征。
	// 注：
	// 1) 调用前，调用者应该知道处于底图下面。
	static std::pair<PatternListPtr, TextStructListPtr> GetSelectedPatterns(const IEditor* pEditor);

	// 判断当前是不是处于底图下选择图元的状态。
	static bool IsUnderSelectCanvasPatternStatus(const IEditor* pEditor);
	/************************************************************************/


	/************************************************************************/
	// 有关报表。

	// 生成报表，如成功则更新pReportData变量。
	static bool GenerateSheetReport(SheetPtr pSheet, ReportDataPtr& pReportData);
	static bool GenerateLabelReport(SheetPtr pSheet, ReportDataPtr& pReportData);
	/************************************************************************/


	/************************************************************************/
	// 有关加工工艺。

	// 分配加工工艺。
	// 注：
	// 1) 该函数只进行数据层面的修改，结束后弹出一个提示消息。
	static void AssignCutFeature(SheetPtr pSheet);

	// 重置加工工艺。
	// 注：
	// 1) 该函数只进行数据层面的修改。
	static void ResetCutFeature(SheetPtr pSheet);

	// 检查并处理加工工艺的干涉。
	// 注：
	// 1) 该函数只进行数据层面的修改。
	static void CheckInterfere(SheetPtr pSheet);

	// 弹出参数界面，重置各种加工工艺。
	// 注：
	// 1) 如果执行了重置，需要返回true。该函数还要在重置后更新几个编辑器中的加工参数。
	static bool ReAssignLeadLine(SheetPtr pSheet, CanvasGeometryEditorPtr pCanvasGeometryEditor, PartLayoutEditorPtr pPartLayoutEditor,
		SequenceEditorPtr pSequenceEditor);
	static bool ReAssignStartCutPoint(SheetPtr pSheet, CanvasGeometryEditorPtr pCanvasGeometryEditor, PartLayoutEditorPtr pPartLayoutEditor,
		SequenceEditorPtr pSequenceEditor);
	static bool ReAssignMicroJoint(SheetPtr pSheet, CanvasGeometryEditorPtr pCanvasGeometryEditor, PartLayoutEditorPtr pPartLayoutEditor,
		SequenceEditorPtr pSequenceEditor);
	static bool ReAssignCornerFeature(SheetPtr pSheet, CanvasGeometryEditorPtr pCanvasGeometryEditor, PartLayoutEditorPtr pPartLayoutEditor,
		SequenceEditorPtr pSequenceEditor);

	// 修改选中轮廓实例的刀具。
	// 注：
	// 1) 不可以传入空的集合，修改完刀具后会判断要不要离开当前action，并且还会刷新视图。
	static void ChangeToolOfSelectedLoopInstances(LoopInstanceListPtr pSelectedLoopInstances, int iToolIndex,
		const IEditor* pCurrentEditor, const PartLayoutEditor* pPartLayoutEditor);

	// 修改选中轮廓实例的内外切。
	// 注：
	// 1) 不可以传入空的集合，修改完内外切后会判断要不要离开当前action，并且还会刷新视图。
	static void ChangeCutSideOfSelectedLoopInstances(LoopInstanceListPtr pSelectedLoopInstances, PatternLoopCutOffsetType emCutOffsetType,
		const IEditor* pCurrentEditor, const PartLayoutEditor* pPartLayoutEditor);

	// 修改选中轮廓实例的切割方向。
	// 注：
	// 1) 不可以传入空的集合，修改完切割方向后会判断要不要离开当前action，并且还会刷新视图。
	static void ChangeCutDirectionOfSelectedLoopInstances(LoopInstanceListPtr pSelectedLoopInstances, CutDirectionChangeType emCutDirectionChangeType,
		const IEditor* pCurrentEditor, const PartLayoutEditor* pPartLayoutEditor);

	// 查询这些轮廓(pLoopToolDatas)所在工序(比如连割工序)中的其他轮廓，通过pLoopToolDataList_other返回它们。
	static void QueryOtherLoopTools(const Sheet* pSheet, const LoopToolDataList* pLoopToolDatas, LoopToolDataList* pOtherLoopToolDatas);

	// 修改选中轮廓实例的缺口。
	// 注：
	// 1) 如没有得到选中的轮廓，则需要报警。
	// 2) 修改完后会判断要不要离开当前action，并且还会刷新视图。
	static void ChangeGapOfSelectedLoopInstances(const IEditor* pCurrentEditor, const PartLayoutEditor* pPartLayoutEditor);
	/************************************************************************/


	/************************************************************************/
	// 有关工序。

	// 自动分配工序。
	// 注：
	// 1) 该函数只进行数据层面的修改，调用前需注意板材上要有未加工的轮廓。
	static void AutoSequence(SheetPtr pSheet, std::set<LONGLONG>& patternLoops_failedToOffset);

	// 清除所有工序。
	// 注：
	// 1) 该函数只进行数据层面的修改。
	static void ClearAllSequence(SheetPtr pSheet);

	// 更新工序并绘制切割路径，在板材数据发生变化时调用。
	static void UpdateSequenceAndDrawCutPath(const Sheet* pSheet, bool bHideSequenceSimulateShapes, LONGLONG& iLatestModifyTimeStamp_ofPartPlacement,
		LONGLONG& iLatestModifyTimeStamp_ofSequenceData, std::map<LONGLONG, CutSequenceDeleteCommandPtr>& sequenceDeleteCommandData,
		SequenceSimulateShapeListPtr& pSequenceSimulateShapes, GlViewPortPtr pViewPort);

	// 基于选中的轮廓创建飞切。
	// 注：
	// 1) 不可以传入空的轮廓集合，如果这些轮廓不符合创建飞切的条件，则会有警告提示。该函数会判断要不要离开当前action，并且还会刷新视图。
	static void AddFlyCut2SelectedLoopInstances(LoopInstanceListPtr pSelectedLoopInstances, const IEditor* pCurrentEditor, const SequenceEditor* pSequenceEditor);
	/************************************************************************/


	/************************************************************************/
	// 有关undo/redo。

	// 更新视图当前的undo/redo列表。
	static void UpdateUndoRedoDropdownList(CMFCRibbonBaseElement* pRibbonElement, CommandListPtr pSheetViewUndoCmds, CommandListPtr pSheetViewRedoCmds);
	/************************************************************************/


	// 生成并显示g代码。
	// 注：
	// 1) 生成g代码并在面板中显示，并进入nc选择action。
	static void GenerateAndViewNcCode(SheetPtr pSheet, MacRouterEditorPtr pMacRouterEditor);

	// 在位编辑板材下的一个零件。
	// 注：
	// 1) 如果没有选中一个零件，那么需要提示用户。
	static void InPlaceEditPart(const PartLayoutEditor* pPartLayoutEditor);

	static void ZoomAll(const Sheet* pSheet, GlViewPortPtr pViewPort);

	// 选择一个dxf并加载成零件。
	// 注:
	// 1) 成功加载了即返回true。
	static bool LoadDxfAsPart(const Sheet* pSheet, PartItemListPtr& pPartItems,
		std::map<LONGLONG, std::vector<std::pair<LONGLONG, Matrix2D>>>& matrixData_of_sameShapePart, FolderNodePtr& pSaveFolder);

	// 对齐选中的轮廓。
	// 注：
	// 1) 不可以传入空的几何特征集合，对齐后会判断要不要离开当前action，并且还会刷新视图。
	// 2) 比如选中两个轮廓的一部分进行对齐，那么就对齐这两个轮廓。
	static void AlignSelectedLoops(std::pair<PatternListPtr, TextStructListPtr> selectedPatterns, AlignLoopStyle emAlignStyle,
		const IEditor* pCurrentEditor, const CanvasGeometryEditor* pCanvasGeometryEditor);

	// 创建右键菜单。
	// 注:
	// 1) 该函数只负责创建菜单项。
	static void CreatePopupMenu(const Sheet* pSheet, const CPoint& point);
};

END_CUTLEADER_NAMESPACE()
