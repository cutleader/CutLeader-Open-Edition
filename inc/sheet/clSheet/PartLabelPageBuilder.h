#pragma once

#include "clSheetCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(ReportTplPage)
DECLARE_CUTLEADER_CLASS(TplEntityList)
DECLARE_CUTLEADER_CLASS(IBindableEntity)
DECLARE_CUTLEADER_CLASS(LabelTplEntity)
DECLARE_CUTLEADER_CLASS(BarCodeTplEntity)
DECLARE_CUTLEADER_CLASS(ReportPage)
DECLARE_CUTLEADER_CLASS(ReportPageList)
DECLARE_CUTLEADER_CLASS(PartItem)
DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(Part)


BEGIN_CUTLEADER_NAMESPACE()

// 对应“零件标签模板页”的报表生成器。
class ClSheet_Export PartLabelPageBuilder
{
public:
	PartLabelPageBuilder(SheetPtr pSheet, ReportTplPagePtr pReportTplPage);
	~PartLabelPageBuilder(void);

public:
	ReportPageListPtr GenRptPageList();

private:
	// 得到模板中的标签模板实体，有且只有一个。
	LabelTplEntityPtr GetLabelEntity();

	// 得到在标签矩形框内的其他实体。
	// 注：
	//  1) 返回这些实体相对标签左上角的偏移，<x方向偏移，y方向偏移>。
	TplEntityListPtr GetInnerEntities(LabelTplEntityPtr pLabelTplEntity, std::vector<std::pair<int,int>>& offsets);

	// 生成一个标签报表实体，加到报表页中。
	/* 参数：
	*  pInnerTplEnts, offsets: 标签模板对象内部的实体及其相对其左上角的偏移。
	*  iCurRowIndex, iCurColumnIndex: 当前的行列。
	*/
	// 注：
	//  1) 只要这一页放得下，这个函数需要更新行列数；如果行列数到了上限，返回false。
	bool AddLabelRptEnt(ReportPagePtr pReportPage, LabelTplEntityPtr pLabelTplEnt, TplEntityListPtr pInnerTplEnts, const std::vector<std::pair<int,int>>& offsets,
		int& iCurRowIndex, int& iCurColumnIndex, const Part* pPart);

	// 得到绑定数据的值。
	CString GetBindDataVal(const Part* pPart, IBindableEntityPtr pBindableEnt);

	// 得到条形码图形。
	// 注：
	//  1) 如果条码内容为空，返回NULL。
	ImgDataPtr GetBarCodeImg(const Part* pPart, BarCodeTplEntityPtr pBarCodeTplEnt);

private:
	SheetPtr m_pSheet;
	ReportTplPagePtr m_pReportTplPage;
};

END_CUTLEADER_NAMESPACE()
