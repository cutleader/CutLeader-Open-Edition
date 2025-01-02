#pragma once

#include "clReportConfigCommon.h"
#include "ClData.h"
#include "FontParam.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClReportConfig_Export TemplateParam : public ClData
{
public:
	TemplateParam(void);

	~TemplateParam(void);

public: // get/set functions.
	void SetPaperWidth(int iWidth) { m_iWidth = iWidth; }
	int GetPaperWidth() { return m_iWidth; }

	void SetPaperHeight(int iHeight) { m_iHeight = iHeight; }
	int GetPaperHeight() { return m_iHeight; }

	void SetLeftMargin(int iLeftMargin) { m_iLeftMargin = iLeftMargin; }
	int GetLeftMargin() { return m_iLeftMargin; }

	void SetRightMargin(int iRightMargin) { m_iRightMargin = iRightMargin; }
	int GetRightMargin() { return m_iRightMargin; }

	void SetTopMargin(int iTopMargin) { m_iTopMargin = iTopMargin; }
	int GetTopMargin() { return m_iTopMargin; }

	void SetBottomMargin(int iBottomMargin) { m_iBottomMargin = iBottomMargin; }
	int GetBottomMargin() { return m_iBottomMargin; }

	TEXT_JUST_TYPE GetTextJustType() const { return m_textJustType; }
	void SetTextJustType(TEXT_JUST_TYPE textJustType) { m_textJustType = textJustType; }

	FontParamPtr GetFontParam() { return m_pFontParam; }
	void SetFontParam(FontParamPtr pFontParam) { m_pFontParam = pFontParam; }

private:
	// the size of the template
	int m_iWidth;
	int m_iHeight;

	// the margin of the paper.
	int m_iLeftMargin;
	int m_iRightMargin;
	int m_iTopMargin;
	int m_iBottomMargin;

	// the text alignment type.
	TEXT_JUST_TYPE m_textJustType;

	// about the font param.
	FontParamPtr m_pFontParam;
};

END_CUTLEADER_NAMESPACE()
