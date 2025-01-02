#pragma once

#include "clReportConfigCommon.h"
#include "RptEntityBase.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(FontParam)
DECLARE_CUTLEADER_CLASS(TextRptEntity)

BEGIN_CUTLEADER_NAMESPACE()

// the text entity in the report.
class ClReportPrinter_Export TextRptEntity : public RptEntityBase
{
public:
	TextRptEntity(void);
	TextRptEntity(const Rect2D& borderRect, CString strContent, TEXT_JUST_TYPE textJustType, FontParamPtr pFontParam);
	~TextRptEntity(void);

public: // implement IRptEntity interface.
	virtual RPT_ENTITY_TYPE GetType() const { return RPT_ENTITY_TEXT; }

public: // get/set functions.
	void SetContent(CString strContent) { m_strContent = strContent; }
	CString GetContent() { return m_strContent; }

	virtual TEXT_JUST_TYPE GetTextJustType() const { return m_textJustType; }
	virtual void SetTextJustType(TEXT_JUST_TYPE textJustType) { m_textJustType = textJustType; }

	virtual FontParamPtr GetFontParam() { return m_pFontParam; }
	virtual void SetFontParam(FontParamPtr pFontParam) { m_pFontParam = pFontParam; }

private:
	// the content.
	CString m_strContent;

	// the text alignment type.
	TEXT_JUST_TYPE m_textJustType;

	// about the font param.
	FontParamPtr m_pFontParam;
};

END_CUTLEADER_NAMESPACE()
