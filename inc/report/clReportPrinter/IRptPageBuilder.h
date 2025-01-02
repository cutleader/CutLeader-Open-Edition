#pragma once

#include "clReportPrinterCommon.h"
#include "BindItem.h"

DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(ColumnData)

BEGIN_CUTLEADER_NAMESPACE()

// “报表页创建器”接口。
class ClReportPrinter_Export IRptPageBuilder
{
public:
	// get the value of the field entity.
	virtual CString GetFieldValue(BIND_ITEM_TYPE bindDataType) = 0;

	// get the image data.
	virtual ImgDataPtr GetImageValue(int Width, int iHeight, BIND_ITEM_TYPE bindDataType) = 0;

	// get a column of data of the table.
	virtual ColumnDataPtr GetTableColData(int iColumnIndex, BIND_ITEM_TYPE tableBindItem, BIND_ITEM_TYPE colBindItem) = 0;
};

END_CUTLEADER_NAMESPACE()
