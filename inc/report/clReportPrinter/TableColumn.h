#pragma once

#include "clReportConfigCommon.h"
#include "clReportPrinterCommon.h"

DECLARE_CUTLEADER_CLASS(FontParam)

BEGIN_CUTLEADER_NAMESPACE()

// 表格某列的格式。
class TableColumn
{
public:
	TableColumn(void);
	~TableColumn(void);

public: // get/set functions.
	void SetColumnTitle(CString strColumnTitle) { m_strColumnTitle = strColumnTitle; }
	CString GetColumnTitle() { return m_strColumnTitle; }

	virtual TEXT_JUST_TYPE GetTextJustType() const { return m_textJustType; }
	virtual void SetTextJustType(TEXT_JUST_TYPE textJustType) { m_textJustType = textJustType; }

	void SetWidth(double dWidth) { m_dWidth = dWidth; }
	double GetWidth() { return m_dWidth; }

	virtual FontParamPtr GetHeaderFont() { return m_pHeaderFont; }
	virtual void SetHeaderFont(FontParamPtr pHeaderFont) { m_pHeaderFont = pHeaderFont; }

	virtual FontParamPtr GetRowFont() { return m_pRowFont; }
	virtual void SetRowFont(FontParamPtr pRowFont) { m_pRowFont = pRowFont; }

private:
	// the title of the column header.
	CString m_strColumnTitle;

	// the text alignment type.
	TEXT_JUST_TYPE m_textJustType;

	// the column width.
	double m_dWidth;

	// the font of the column header.
	FontParamPtr m_pHeaderFont;

	// the font of the content.
	FontParamPtr m_pRowFont;
};

END_CUTLEADER_NAMESPACE()
