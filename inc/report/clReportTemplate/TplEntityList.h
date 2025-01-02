#pragma once

#include "ClData.h"
#include "ITplEntity.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(TplEntityList)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export TplEntityList : public std::vector<ITplEntityPtr>,
										 public ClData
{
public:
	TplEntityList(void);
	virtual ~TplEntityList(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;
	virtual IDataPtr Clone() const override;

public:
	/************************************************************************/
	// about get template entity.

	// get template entities which intersect with "rect".
	TplEntityListPtr GetEntityInRect(CRect rect);

	// get template entity index.
	int GetEntityIndex(ITplEntityPtr pTplEntity);

	// get template entities by the entity type.
	TplEntityListPtr GetEntityByType(ENTITY_TYPE entityType);

	// get the entity by ID.
	ITplEntityPtr GetEntityByID(LONGLONG iEntityID);
	/************************************************************************/

	/************************************************************************/
	// about the position of the template entities.

	// get the left top point of the entity list.
	void GetLeftTopPt(double& dLeft, double& dTop);

	// offset the entity list.
	void OffesetEntity(double dOffsetX, double dOffsetY);
	/************************************************************************/

	// append the entity list.
	// notes:
	//   1) the duplicated entities will not be added.
	void AddEntity(TplEntityListPtr pTplEntityList);

	void DeleteEntity(ITplEntityPtr pTplEntity);
	void DeleteEntity(TplEntityListPtr pTplEntityList);

	BOOL IsEntityExist(ITplEntityPtr pTplEntity);
};

END_CUTLEADER_NAMESPACE()
