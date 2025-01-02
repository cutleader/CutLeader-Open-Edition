#pragma once

#include "clReportTemplateCommon.h"

DECLARE_CUTLEADER_CLASS(BindItemList)

BEGIN_CUTLEADER_NAMESPACE()

// 可用的绑定项。
// 注：
//   1) 按照零件/板材/排版作业/材料来分类，各个模板页从中挑选各自的绑定项。
typedef enum tagBindItemType
{
	BIND_ITEM_NOT_INIT = -1,


	/************************************************************************/
	// for part.

	// 零件表格。
	BIND_ITEM_PART_LIST = 0,

	// basic info of part.
	BIND_ITEM_PART_NAME = 10,
	BIND_ITEM_PART_COUNT = 11,
	BIND_ITEM_PART_PREVIEW = 12,
	BIND_ITEM_PART_SIZE = 13,
	BIND_ITEM_PART_AREA = 14,
	BIND_ITEM_PART_WEIGHT = 15,

	// nesting info.
	BIND_ITEM_PART_SUBMIT_COUNT = 20,
	BIND_ITEM_PART_NESTED_COUNT = 21,
	/************************************************************************/


	/************************************************************************/
	// 关于板材。

	// 板材表格。
	BIND_ITEM_SHEET_LIST = 100,

	// 基本信息。
	BIND_ITEM_SHEET_NAME = 110,
	BIND_ITEM_SHEET_SIZE = 111,
	BIND_ITEM_SHEET_UTILIZATION = 112,
	BIND_ITEM_SHEET_COUNT = 113,
	BIND_ITEM_SHEET_CREATE_DATE = 114,

	// 切割信息统计。
	BIND_ITEM_SHEET_CUT_LENGTH = 120,
	BIND_ITEM_SHEET_CONTOUR_LENGTH = 121,
	BIND_ITEM_SHEET_RAPID_LENGTH = 122,
	BIND_ITEM_SHEET_PIERCE_COUNT = 123,
	BIND_ITEM_SHEET_TOTAL_LENGTH = 124,
	BIND_ITEM_SHEET_CUT_TIME = 125,
	BIND_ITEM_SHEET_RAPID_TIME = 126,
	BIND_ITEM_SHEET_PIERCE_TIME = 127,
	BIND_ITEM_SHEET_CUT_COST = 128,
	BIND_ITEM_SHEET_PIERCE_COST = 129,
	BIND_ITEM_SHEET_RAPID_COST = 130,
	BIND_ITEM_SHEET_TOTAL_COST = 131,
	BIND_ITEM_SHEET_TOTAL_TIME = 132, // 总时间

	// 板材预览。
	BIND_ITEM_SHEET_PREVIEW = 140,
	/************************************************************************/


	/************************************************************************/
	// 关于材料

	// tables.
	BIND_ITEM_MATSIZE_LIST = 200,

	// basic info of material.
	BIND_ITEM_MATSIZE_TYPE = 210,
	BIND_ITEM_MATSIZE_NAME = 211,
	BIND_ITEM_MATSIZE_SIZE = 212,
	BIND_ITEM_MATSIZE_COUNT = 213,
	BIND_ITEM_MATSIZE_AVAIL = 214,

	// nesting info.
	BIND_ITEM_MATSIZE_SUBMIT_COUNT = 220,
	BIND_ITEM_MATSIZE_NESTED_COUNT = 221,
	/************************************************************************/


	/************************************************************************/
	// 任务方面

	// tables.

	// 任务基本信息
	BIND_ITEM_TASK_NAME = 310,
	BIND_ITEM_TASK_COMMENT = 311,
	BIND_ITEM_TASK_CREATE_DATE = 312,
	BIND_ITEM_TaskUtilization = 313,
	BIND_ITEM_TASK_TotalSheetCount = 315,
	BIND_ITEM_Task_InitialPartCount = 316,
	BIND_ITEM_Task_NestedPartCount = 317, // 任务中某种零件的已排数量
	/************************************************************************/


	// 公用。
	BIND_ITEM_INDEX = 350,
	BIND_ITEM_PRINT_DATE = 351,
} BIND_ITEM_TYPE;

// the available control types for "bind item".
typedef enum tagBindItemCtrlType
{
	BIND_ITEM_CTRL_TEXT = 0,
	BIND_ITEM_CTRL_IMAGE = 1,
	BIND_ITEM_CTRL_TABLE = 2,
	BIND_ITEM_CTRL_BARCODE = 3,
} BIND_ITEM_CTRL_TYPE;

// 绑定项。
class BindItem
{
public:
	BindItem(void);
	BindItem(BIND_ITEM_TYPE bindItemType, BIND_ITEM_CTRL_TYPE bindItemCtrlType, CString strBindItemName);
	~BindItem(void);

public: // get/set functions.
	void SetBindItemType(BIND_ITEM_TYPE bindItemType) { m_bindItemType = bindItemType; }
	BIND_ITEM_TYPE GetBindItemType() { return m_bindItemType; }

	void SetBindItemCtrlType(BIND_ITEM_CTRL_TYPE bindItemCtrlType) { m_bindItemCtrlType = bindItemCtrlType; }
	BIND_ITEM_CTRL_TYPE GetBindItemCtrlType() { return m_bindItemCtrlType; }

	void SetBindItemName(CString strBindItemName) { m_strBindItemName = strBindItemName; }
	CString GetBindItemName() { return m_strBindItemName; }

	void HasSubItem(BOOL bHasSubItem) { m_bHasSubItem = bHasSubItem; }
	BOOL HasSubItem() { return m_bHasSubItem; }

	void SetSubBindItemList(BindItemListPtr pSubBindItemList) { m_pSubBindItemList = pSubBindItemList; }
	BindItemListPtr GetSubBindItemList() { return m_pSubBindItemList; }

private:
	// the "bind item" type.
	BIND_ITEM_TYPE m_bindItemType;

	// the control type of the "bind item".
	BIND_ITEM_CTRL_TYPE m_bindItemCtrlType;

	// the name of the bind item.
	CString m_strBindItemName;

	// whether this bind item has the sub-items, e.g. table item.
	BOOL m_bHasSubItem;

	// the sub-items.
	BindItemListPtr m_pSubBindItemList;
};

END_CUTLEADER_NAMESPACE()
