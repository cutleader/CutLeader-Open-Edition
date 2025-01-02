#pragma once

#include "clFrameCommon.h"

DECLARE_CUTLEADER_CLASS(DataItem)
DECLARE_CUTLEADER_CLASS(PartDocument)
DECLARE_CUTLEADER_CLASS(TaskDocument)
DECLARE_CUTLEADER_CLASS(CLeaderDoc)
DECLARE_CUTLEADER_CLASS(CLeaderView)
DECLARE_CUTLEADER_CLASS(PartView)
DECLARE_CUTLEADER_CLASS(StartPageView)


BEGIN_CUTLEADER_NAMESPACE()

// this class care something about the documents.
class CLFRAME_Export DocManager
{
public:
	/************************************************************************/
	// 关于新建文档。

	// 新建零件。
	static void NewPartDoc(CMultiDocTemplate* pPartDocTemplate);
	static void NewPartDocFromDxfDwg(CMultiDocTemplate* pPartDocTemplate);

	// 处于在位编辑零件时，新建零件。
	static void NewPartDocUnderPartInplaceEditMode(CMultiDocTemplate* pPartDocTemplate);
	static void NewPartDocFromDxfDwgUnderPartInplaceEditMode(CMultiDocTemplate* pPartDocTemplate);

	// 通过新建任务对话框新建任务文档。
	static void NewTaskDoc(CMultiDocTemplate* pTaskDocTemplate);
	// 按照单文档的方式新建任务。 新建任务之前，该函数会检查当前最多只能打开了一个任务文档，并会保存/关闭这个文档。
	static void NewTaskDocBySingleDocMode(CMultiDocTemplate* pTaskDocTemplate);

	// 通过自动排版新建任务文档。
	// 注：
	// 1) bSingleDocMode: 为true时，按照单文档的方式新建任务。 新建任务之前，该函数会检查当前最多只能打开了一个任务文档，并会保存/关闭这个文档。
	static void NewTaskDocFromNesting(CMultiDocTemplate* pTaskDocTemplate, bool bSingleDocMode);

	// 根据默认参数配置新建一个任务文档。
	static void NewDefaultTaskDoc(CMultiDocTemplate* pTaskDocTemplate);

	// 新建任务，把dxf放在底图上。
	static void NewTaskAndAddDxf2Canvas(CMultiDocTemplate* pTaskDocTemplate, const CString& strDxfFilePath);

	// 设置默认的加工参数和材料尺寸。
	static void SetDefaultParam();
	/************************************************************************/


	/************************************************************************/
	// 关于打开文档。

	// 打开零件文档。
	static void OpenPartDoc(DataItemPtr pDataItem, CMultiDocTemplate* pPartDocTemplate);

	// 处于在位编辑零件时，打开零件文档。
	static void OpenPartDocUnderPartInplaceEditMode(DataItemPtr pDataItem, CMultiDocTemplate* pPartDocTemplate);

	// 打开任务文档。
	static void OpenTaskDoc(DataItemPtr pDataItem, CMultiDocTemplate* pTaskDocTemplate);
	// 按照单文档的方式打开任务。打开任务之前，该函数会检查当前最多只能打开了一个任务文档，并会保存/关闭这个文档。
	static void OpenTaskDocBySingleDocMode(DataItemPtr pDataItem, CMultiDocTemplate* pTaskDocTemplate);

	// 打开指定任务并激活指定板材。
	// 注：
	// 1) 打开这个任务前需要确保此任务没有打开。
	// 2) 这个函数没有更新“最近打开的文档”列表。
	static void OpenSpecifiedSheet(CMultiDocTemplate* pTaskDocTemplate, long long iTaskID, long long iSheetID);
	/************************************************************************/


	/************************************************************************/
	// 关于保存文档。

	// save as the part in a location.
	static void SaveAsPart(PartDocument* pPartDoc);

	// save as the task in a location.
	static void SaveAsTask(TaskDocument* pTaskDoc);
	/************************************************************************/


	/************************************************************************/
	// 关于激活文档。

	// if "pDataItem" is opened, active it and return TRUE.
	static BOOL ActivePartDoc(DataItemPtr pDataItem, CMultiDocTemplate* pPartDocTemplate);
	static BOOL ActivePartDoc(LONGLONG iPartID, CMultiDocTemplate* pPartDocTemplate);

	// if "pDataItem" is opened, active it and return TRUE.
	static BOOL ActiveTaskDoc(DataItemPtr pDataItem, CMultiDocTemplate* pTaskDocTemplate);
	/************************************************************************/


	// get active document.
	static CLeaderDoc* GetActiveDocument();

	// 得到当前零件视图，ClWindowFinder中也有类似函数。
	// 注：
	// 1）调用时需要确保当前视图是零件视图，否则该函数内报错。
	static PartView* GetActivePartView();

	// 得到当前可交互视图，是零件视图或板材视图。当前要是没有打开零件或任务，返回空。
	static CLeaderView* GetCurrentView(CString& strViewName);

private:
	// 关闭当前任务文档。
	// 注：
	// 1) 调用该函数前需要确保此时cam只打开了一个任务文档。
	// 2) 如果该任务被修改了，该函数提示用户是否保存，用户可以选择不保存。
	static void CloseCurrentTaskDoc();

	// 在另存了文档后，更新文档的访问历史。
	// 注：
	// 1) iOldDataID是另存前的数据ID(如零件或任务ID)，pNewDataItem是另存结束后生成的新数据项。
	static void UpdateDocHistoryAfterSaveAs(LONGLONG iOldDataID, DataItemPtr pNewDataItem);

	// 在打开了文档后，更新文档的访问历史。
	// 注：
	// 1) pDataItem是要打开的数据项。
	static void UpdateDocHistoryAfterOpen(DataItemPtr pDataItem);
};

END_CUTLEADER_NAMESPACE()
