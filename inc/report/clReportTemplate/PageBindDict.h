#pragma once

#include "clReportTemplateCommon.h"

DECLARE_CUTLEADER_CLASS(ExtPropTypeList)
DECLARE_CUTLEADER_CLASS(PageBindDict)
DECLARE_CUTLEADER_CLASS(BindItemList)

BEGIN_CUTLEADER_NAMESPACE()

// this class defined the bindable data for specified template page.
class ClReportTemplate_Export PageBindDict
{
public:
	PageBindDict(void);
	~PageBindDict(void);

public: // get/set functions.
	void SetTplPageType(REPORT_TPL_PAGE_TYPE tplPageType) { m_tplPageType = tplPageType; }
	REPORT_TPL_PAGE_TYPE GetTplPageType() { return m_tplPageType; }

	void SetBindItemList(BindItemListPtr pBindItemList) { m_pBindItemList = pBindItemList; }
	BindItemListPtr GetBindItemList() { return m_pBindItemList; }

	void SetExtPropTypeList(ExtPropTypeListPtr pExtPropTypeList) { m_pExtPropTypeList = pExtPropTypeList; }
	ExtPropTypeListPtr GetExtPropTypeList() { return m_pExtPropTypeList; }

public:
	// build the PageBindDict by the page type.
	// notes:
	//   1) in this function, we will decide the bindable items for the template page, there are some rules
	//      we should follow, e.g. we can bind to "nest part list" table in "nest job info" page and cannot bind
	//      to "part list" table, but we can bind "part list" table in "sheet info" page.
	static PageBindDictPtr BuildPageBindDict(REPORT_TPL_PAGE_TYPE tplPageType);

private:
	REPORT_TPL_PAGE_TYPE m_tplPageType;

	// the bindable data for this template page.
	BindItemListPtr m_pBindItemList;

	// 这个模板页可用的扩展属性。
	// 注：
	//  1) 构造函数中初始化空容器。
	ExtPropTypeListPtr m_pExtPropTypeList;
};

END_CUTLEADER_NAMESPACE()
