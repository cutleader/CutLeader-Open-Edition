#pragma once

#include "clMaterialLibCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(MaterialThickness)
DECLARE_CUTLEADER_CLASS(MaterialSizeList)
DECLARE_CUTLEADER_CLASS(MaterialThicknessList)

BEGIN_CUTLEADER_NAMESPACE()

// 材料厚度。
// 注意:
//   1) 该对象的父ID为“材料”ID。
class ClMaterialLib_Export MaterialThickness : public ClData
{
public:
	MaterialThickness(void);
	MaterialThickness(LONGLONG iID, LONGLONG iParentID, CString strName, double dThickness, CString strComment);
	virtual ~MaterialThickness(void);

public: // implement IData interface
	IDataPtr Clone() const override;

public: // get/set functions.
	void SetName(CString strName) { m_strName = strName; }
	CString GetName() const { return m_strName; }

	void SetComment(CString strComment) { m_strComment = strComment; }
	CString GetComment() const { return m_strComment; }

	void SetThickness(double dMatThickness) { m_dMatThickness = dMatThickness; }
	double GetThickness() const { return m_dMatThickness; }

	void SetMatSizeList(MaterialSizeListPtr pMaterialSizeList) { m_pMatSizeList = pMaterialSizeList; }
	MaterialSizeListPtr GetMatSizeList() const { return m_pMatSizeList; }

private:
	// 名称。
	CString m_strName;

	// the comment
	CString m_strComment;

	// the thickness
	double m_dMatThickness;

	// the material sizes of this material name.
	MaterialSizeListPtr m_pMatSizeList;
};

class ClMaterialLib_Export MaterialThicknessList : public ClData, public std::vector<MaterialThicknessPtr>
{
public:
	MaterialThicknessList(void);
	virtual ~MaterialThicknessList(void);

public: // implement IData interface
	IDataPtr Clone() const override;

public:
	void DeleteItemByID(LONGLONG iId);

	// get the material name object by the ID.
	MaterialThicknessPtr GetItemByID(LONGLONG iId);
};

END_CUTLEADER_NAMESPACE()
