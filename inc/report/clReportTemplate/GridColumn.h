#pragma once

#include "TextTplEntityBase.h"
#include "BindableEntityBase.h"

DECLARE_CUTLEADER_CLASS(GridColumn)

BEGIN_CUTLEADER_NAMESPACE()

// 表格某列的定义。
class ClReportTemplate_Export GridColumn : virtual public TextTplEntityBase,
				   virtual public BindableEntityBase
{
public:
	GridColumn(void);
	virtual ~GridColumn(void);

public: // functions from IData interface.
	virtual IDataPtr Clone() const override;

public: // implement ITplEntity interface.
	virtual ENTITY_TYPE GetType() const { return ENTITY_GRID_COLUMN; }

public: // get/set functions.
	void SetColumnTitle(CString strColumnTitle) { m_strColumnTitle = strColumnTitle; }
	CString GetColumnTitle() { return m_strColumnTitle; }

	void SetWidth(int iWidth) { m_iWidth = iWidth; }
	int GetWidth() { return m_iWidth; }

	virtual FontParamPtr GetColHeaderFont() { return m_pHeaderFont; }
	virtual void SetColHeaderFont(FontParamPtr pFontParam) { m_pHeaderFont = pFontParam; }

private:
	// the title of the column header.
	CString m_strColumnTitle;

	// the column width.
	// notes:
	//   1) use intData9.
	int m_iWidth;

	// the font of the column header.
	// notes:
	//   1) stored in intData10~intData16, and strData3.
	FontParamPtr m_pHeaderFont;
};

END_CUTLEADER_NAMESPACE()
