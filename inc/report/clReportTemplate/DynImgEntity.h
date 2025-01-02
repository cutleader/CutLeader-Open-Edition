#pragma once

#include "BorderTplEntityBase.h"
#include "BindableEntityBase.h"

DECLARE_CUTLEADER_CLASS(DynImgEntity)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export DynImgEntity : virtual public BorderTplEntityBase,
					 virtual public BindableEntityBase
{
public:
	DynImgEntity(void);
	~DynImgEntity(void);

public: // functions from IData interface.
	virtual IDataPtr Clone() const override;

public: // implement ITplEntity interface.
	virtual ENTITY_TYPE GetType() const { return ENTITY_DYN_IMG; }
};

END_CUTLEADER_NAMESPACE()
