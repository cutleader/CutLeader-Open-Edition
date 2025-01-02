#pragma once

#include "IRptPageBuilder.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ReportTplPage)
DECLARE_CUTLEADER_CLASS(ITplEntity)
DECLARE_CUTLEADER_CLASS(StaticTplEntity)
DECLARE_CUTLEADER_CLASS(FieldTplEntity)
DECLARE_CUTLEADER_CLASS(StaticImgEntity)
DECLARE_CUTLEADER_CLASS(DynImgEntity)
DECLARE_CUTLEADER_CLASS(GridTplEntity)
DECLARE_CUTLEADER_CLASS(LineTplEntity)
DECLARE_CUTLEADER_CLASS(BoxTplEntity)
DECLARE_CUTLEADER_CLASS(EllipseTplEntity)
DECLARE_CUTLEADER_CLASS(TplEntityList)
DECLARE_CUTLEADER_CLASS(ReportPage)
DECLARE_CUTLEADER_CLASS(ReportPageList)
DECLARE_CUTLEADER_CLASS(IRptEntity)
DECLARE_CUTLEADER_CLASS(TextRptEntity)
DECLARE_CUTLEADER_CLASS(ImgRptEntity)
DECLARE_CUTLEADER_CLASS(LineRptEntity)
DECLARE_CUTLEADER_CLASS(BoxRptEntity)
DECLARE_CUTLEADER_CLASS(EllipseRptEntity)
DECLARE_CUTLEADER_CLASS(TableRptEntity)
DECLARE_CUTLEADER_CLASS(ColumnDataList)

BEGIN_CUTLEADER_NAMESPACE()

// the table info used for spread table.
struct TableInfo
{
	// the header/row height of the table.
	int iHeaderHeight;
	int iRowHeight;

	// the actually data row count.
	int iDataRowCount;
};

// the result info for spread table.
struct TableSpreadInfo
{
	// the occupied pages count.
	int iOccupyPageCount;

	// the row counts in each occupied page.
	std::vector<int> rowCountList;

	// the end position of the table on the last page.
	int iEndPos;
};

// “报表页创建器”基类。
class ClReportPrinter_Export RptPageBuilderBase : public IRptPageBuilder
{
public:
	RptPageBuilderBase(void);
	~RptPageBuilderBase(void);

public:
	// 从“报表模板页”生成“报表页”。
	ReportPageListPtr GenRptPageList(ReportTplPagePtr pReportTplPage);

private:
	/************************************************************************/
	// 关于创建“报表实体”。

	// build the report entity from "pTplEntity".
	/* params:
	*  iEntityTopPos: position where put the entity on "pCurReportPage".
	*  iEntityBtmPos: the bottom position of the new entity on report page.
	*/
	// notes:
	//   1) normally, the new report entity will be placed in "pReportPage" and "pReportPage" will be returned.
	//      but sometimes "pReportPage" do not have enough place to hold table rows, so new report pages will be generated,
	//      at this time the last report page will returned to caller.
	ReportPagePtr BuildRptEntity(ReportTplPagePtr pReportTplPage, ITplEntityPtr pTplEntity, 
								 ReportPagePtr pCurReportPage, int iEntityTopPos, ReportPageListPtr pReportPageList, int& iEntityBtmPos);

	// build the static report entity.
	// notes:
	//   1) the report entity will be created on "iEntityTopPos", you know, the position of report
	//      entity may not be same with that in "pTplEntity".
	IRptEntityPtr BuildStaticRptEntity(ITplEntityPtr pTplEntity, int iEntityTopPos);

	// build text report entity from the label template entity.
	TextRptEntityPtr BuildTextEntity(StaticTplEntityPtr pStaticTplEntity, int iEntityTopPos);

	// build text report entity from the field template entity.
	TextRptEntityPtr BuildTextEntity(FieldTplEntityPtr pFieldTplEntity, int iEntityTopPos, CString strContent);

	// build image report entity from the static image template entity.
	ImgRptEntityPtr BuildImgEntity(StaticImgEntityPtr pStaticImgEntity, int iEntityTopPos);

	// build image report entity from the dynamic image template entity.
	ImgRptEntityPtr BuildImgEntity(DynImgEntityPtr pDynImgEntity, int iEntityTopPos, ImgDataPtr pImgData);

	// build line report entity from the line template entity.
	LineRptEntityPtr BuildLineEntity(LineTplEntityPtr pLineTplEntity, int iEntityTopPos);

	// build box report entity from the box template entity.
	BoxRptEntityPtr BuildBoxEntity(BoxTplEntityPtr pBoxTplEntity, int iEntityTopPos);

	// build ellipse report entity from the ellipse template entity.
	EllipseRptEntityPtr BuildEllipseEntity(EllipseTplEntityPtr pEllipseTplEntity, int iEntityTopPos);

	// build table report entity.
	TableRptEntityPtr BuildTableEntity(GridTplEntityPtr pGridTplEntity, int iEntityTopPos, ColumnDataListPtr pColumnDataList);

	// get the data which should fill table "pGridTplEntity".
	// notes:
	//   1) one item in the return list stand for a column data.
	ColumnDataListPtr GetTableData(GridTplEntityPtr pGridTplEntity);

	// spread the table from "iStartPos" of the page, return the used page count and the end position.
	/* params:
	*  pReportTplPage: the template page.
	*  iStartPos: the start position of the start page.
	*  tableInfo: the table info used for spread table.
	*  tableSpreadInfo: the result info for spread table.
	*/
	void SpreadTable(ReportTplPagePtr pReportTplPage, int iStartPos, TableInfo tableInfo, TableSpreadInfo& tableSpreadInfo);
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
