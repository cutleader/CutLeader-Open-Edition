#pragma once

#include "clMaterialLibCommon.h"
#include "BusinessData.h"

DECLARE_CUTLEADER_CLASS(MaterialThicknessList)
DECLARE_CUTLEADER_CLASS(Material)

BEGIN_CUTLEADER_NAMESPACE()

// “材料”对象。
class ClMaterialLib_Export Material : public BusinessData
{
public:
	Material(void);
	virtual ~Material(void);

public: // implement IData interface
	virtual IDataPtr Clone() const override;

public: // implement IBusinessData interface
	virtual BusinessDataType GetBusinessDataType() const override { return BusinessData_Material; }

public: // get/set functions.
	void SetMatThickList(MaterialThicknessListPtr pMatThickList) { m_pMatThickList = pMatThickList; }
	MaterialThicknessListPtr GetMatThickList() const { return m_pMatThickList; }

	void SetGravity(double dGravity) { m_dGravity = dGravity; }
	double GetGravity() const { return m_dGravity; }

	void IsVisible(BOOL bVisible) { m_bVisible = bVisible; }
	BOOL IsVisible() const { return m_bVisible; }

private:
	// 材料比重。
	double m_dGravity;

	// “材料厚度”集合。
	MaterialThicknessListPtr m_pMatThickList;

	// 是否可见。
	BOOL m_bVisible;
};

class ClMaterialLib_Export MaterialList : public ClData,
								   public std::vector<MaterialPtr>
{
public:
	MaterialList(void);
	~MaterialList(void);

public:
	void DeleteItemByID(LONGLONG iId);

	// get the material type object by the ID.
	MaterialPtr GetItemByID(LONGLONG iId);
};

END_CUTLEADER_NAMESPACE()
