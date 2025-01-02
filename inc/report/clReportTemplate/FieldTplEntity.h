#pragma once

#include "TextTplEntityBase.h"
#include "BindableEntityBase.h"

DECLARE_CUTLEADER_CLASS(FieldTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export FieldTplEntity : virtual public TextTplEntityBase,
					   virtual public BindableEntityBase
{
public:
	FieldTplEntity(void);
	~FieldTplEntity(void);

public: // functions from IData interface.
	virtual IDataPtr Clone() const override;

public: // implement ITplEntity interface.
	virtual ENTITY_TYPE GetType() const { return ENTITY_FIELD; }
};

END_CUTLEADER_NAMESPACE()
