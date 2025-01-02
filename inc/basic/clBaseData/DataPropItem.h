#pragma once

#include "clBaseDataCommon.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

typedef boost::shared_ptr<std::vector<CString>> StringListPtr;

// the property item of the data.
class ClBaseData_Export DataPropItem
{
public:
	DataPropItem();
	DataPropItem(const DataPropItem& propItem);
	DataPropItem(const CComVariant& var);
	DataPropItem(int iItemName, const CComVariant& var);
	DataPropItem(int iCatagoryName, int iItemName, const CComVariant& var);
	DataPropItem(int iCatagoryName, int iItemName, const CComVariant& var, CString strFormat);
	DataPropItem(int iCatagoryName, int iItemName, const CComVariant& var, int iPrecision); // iPrecision: the precision for float number.
	DataPropItem(int iCatagoryName, int iItemName, const CComVariant& var, StringListPtr optsPtr); // constructor for option-type item.
	~DataPropItem(void);
	
public: // get/set functions.
	/************************************************************************/
	// basic info of data prop item.

	// get the category name
	int GetCategoryName() const { return m_iCatagoryName; }

	void SetItemName(int& i) { m_iItemName = i; }
	int GetItemName() const { return m_iItemName; }

	void SetValue(const CComVariant& v) { m_value = v; }
	const CComVariant& GetValue() const { return m_value; }
	/************************************************************************/


	/************************************************************************/
	// for number type item.

	void IsNumber(BOOL bNumber) { m_bNumber = bNumber; }
	BOOL IsNumber() const { return m_bNumber; }

	const CString& GetFormat() const { return m_strFormat; }

	void CanNegative(BOOL bCanNegative) { m_bCanNegative = bCanNegative; }
	BOOL CanNegative() const { return m_bCanNegative; }
	/************************************************************************/


	/************************************************************************/
	// for option type item.

	void IsOption(BOOL bOption) { m_bOption = bOption; }
	BOOL IsOption() const { return m_bOption; }

	void SetOptions(StringListPtr strOptions) { m_strOptions = strOptions; }
	StringListPtr GetOptions() const { return m_strOptions; }
	/************************************************************************/


	void HasButton(BOOL bHasButton) { m_bHasButton = bHasButton; }
	BOOL HasButton() const { return m_bHasButton; }

	void IsReadOnly(BOOL bReadOnly) { m_bReadOnly = bReadOnly; }
	BOOL IsReadOnly() const { return m_bReadOnly; }

private:
	void CommonConstruct();

private:
	/************************************************************************/
	// basic info of data prop item.

	// the variable indicate the category of the prop item.
	int m_iCatagoryName;

	// the property item name
	int m_iItemName;

	// the value
	CComVariant m_value;
	/************************************************************************/


	/************************************************************************/
	// for number type item.

	// whether this item is num type item.
	BOOL m_bNumber;

	// the format for number-type item.
	// motes:
	//   1) we can use it to decide whether this is a integer or float number.
	CString m_strFormat;

	// whether the number can be negative.
	BOOL m_bCanNegative;
	/************************************************************************/


	/************************************************************************/
	// for option type item.

	// whether this is an option item.
	BOOL m_bOption;

	// the options.
	// notes:
	//   1) the memory of this variable will be allocated outside this class.
	StringListPtr m_strOptions;
	/************************************************************************/


	// whether this item has a button.
	BOOL m_bHasButton;

	// whether this item is read only.
	BOOL m_bReadOnly;
};

END_CUTLEADER_NAMESPACE()
