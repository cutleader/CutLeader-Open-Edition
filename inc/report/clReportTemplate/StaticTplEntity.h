#pragma once

#include "TextTplEntityBase.h"

DECLARE_CUTLEADER_CLASS(StaticTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

// ¾²Ì¬ÎÄ±¾¡£
class ClReportTemplate_Export StaticTplEntity : public TextTplEntityBase
{
public:
	StaticTplEntity(void);
	~StaticTplEntity(void);

public: // functions from IData interface.
	virtual IDataPtr Clone() const override;

public: // implement ITplEntity interface.
	virtual ENTITY_TYPE GetType() const { return ENTITY_STATIC; }

public: // get/set functions.
	void SetTitle(CString strTitle) { m_strTitle = strTitle; }
	CString GetTitle() { return m_strTitle; }

private:
	// the title of the label.
	CString m_strTitle;
};

END_CUTLEADER_NAMESPACE()
