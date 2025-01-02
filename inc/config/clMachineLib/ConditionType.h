#pragma once

#include "clUtilityCommon.h"
#include "clMachineLibCommon.h"
#include "baseConst.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(ConditionType)

BEGIN_CUTLEADER_NAMESPACE()

typedef enum tagCondCategoryType
{
	COND_CATEGORY_CUT		= 0,
	COND_CATEGORY_DOPIERCE	= 1,
	COND_CATEGORY_UNPIERCE	= 2
} COND_CATEGORY_TYPE;

// 条件类型。
class ClMachineLib_Export ConditionType : public ClData
{
public:
	ConditionType(void);
	ConditionType(CString condName, COND_CATEGORY_TYPE condCateType = COND_CATEGORY_CUT, BASIC_DATA_TYPE condDataType = BASIC_DATA_INT, CString comment = EMPTY_STRING);
	virtual ~ConditionType(void);

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // get/set functions.
	void SetCondName(CString condName) { m_condName = condName; }
	CString GetCondName() { return m_condName; }

	void SetCondCategoryType(COND_CATEGORY_TYPE condCateType) { m_condCateType = condCateType; }
	COND_CATEGORY_TYPE GetCondCategoryType() { return m_condCateType; }

	void SetCondDataType(BASIC_DATA_TYPE condDataType) { m_condDataType  =condDataType; }
	BASIC_DATA_TYPE GetCondDataType() { return m_condDataType; }

	void SetCondComment(CString comment) { m_comment = comment; }
	CString GetCondComment() { return m_comment; }

	void SetUniqueID(int iUniqueID) { m_iUniqueID = iUniqueID; }
	int GetUniqueID() { return m_iUniqueID; }

public:
	// get names of all category types of conditions.
	// e.g. "Cut Condition","Do-Pierce Condition",...
	static StringListPtr GetAllCategoryType();

	// get names of all data types of conditions.
	// e.g. "String","Integer",...
	static StringListPtr GetAllDataType();

public:
	// get the "string" name of the category.
	// e.g., return "Cut Condition",...
	CString GetCateName();

	// parse the condition category from string
	void SetCondCategory(CString str);

	// get the "string" name of the data type.
	// e.g., return "String","Integer",...
	CString GetDataTypeName();

	// parse the condition type from string
	void SetCondDataType(CString str);

	// get the default value of this condition type.
	CComVariant GetDefaultVal();

private:
	void CommonConstruct();

private:
	// the name of the condition
	CString m_condName;

	// the category of the condition.
	COND_CATEGORY_TYPE m_condCateType;

	// the data type of the condition.
	BASIC_DATA_TYPE m_condDataType;

	// this ID will be used by driver.
	int m_iUniqueID;

	// the comment of the condition.
	CString m_comment;
};

END_CUTLEADER_NAMESPACE()
