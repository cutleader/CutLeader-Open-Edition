#pragma once

#include "clUtilityCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IntegerList)

BEGIN_CUTLEADER_NAMESPACE()

class ClUtility_Export IntegerList
{
public:
	IntegerList();
	~IntegerList();

public:
	/************************************************************************/
	// with other IntegerList.

	// whether "pIntList1" include "pIntList2".
	// notes:
	//   1) if we can find each item of "pIntList2" in "pIntList1", we think "pIntList1" include "pIntList2".
	BOOL Include(IntegerListPtr pIntList) const;

	// notes:
	//   1) if we can find each item of "pIntList2" in "pIntList1", and they have the same count, we think "pIntList1" is same with "pIntList2". we do not care the order.
	BOOL SameWith(IntegerListPtr pIntList) const;
	/************************************************************************/


	// remove.
	void RemoveInt(LONGLONG iInteger);
	void RemoveIntList(IntegerListPtr pIntegerList);

	BOOL Include(LONGLONG iIntVal) const;

	unsigned int size() const { return m_intVector.size(); }
	LONGLONG at(int iIndex) const { return m_intVector.at(iIndex); }
	void AddInt(LONGLONG iValue) { m_intVector.push_back(iValue); }
	std::vector<LONGLONG>::const_iterator begin() const { return m_intVector.begin(); }
	std::vector<LONGLONG>::const_iterator end() const { return m_intVector.end(); }
	void InsertIntList(IntegerListPtr pIntegerList) { m_intVector.insert(m_intVector.begin(), pIntegerList->begin(), pIntegerList->end()); }

private:
	std::vector<LONGLONG> m_intVector;
};

END_CUTLEADER_NAMESPACE()
