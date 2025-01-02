#pragma once

#include "clNestJobCommon.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

typedef boost::shared_ptr<std::vector<CString>> StringListPtr;

class ClNestJob_Export NestPriority
{
public:
	NestPriority(void);
	NestPriority(int iNestPriority);

public:
	static NestPriority MinPriority() { return NestPriority(MinVal()); }
	static NestPriority MaxPriority() { return NestPriority(MaxVal()); }

	static StringListPtr GetAllPriority();

	static int Count() { return 10; }
	static int MinVal() { return 1; }
	static int MaxVal() { return 10; }

public:
	BOOL operator == (const NestPriority& priority);
	BOOL operator >= (const NestPriority& priority);
	BOOL operator > (const NestPriority& priority);

	void operator ++(int) { m_iNestPriority++; }
	void operator --(int) { m_iNestPriority--; }

public: // get/set functions.
	int GetVal() const { return m_iNestPriority; }
	void SetVal(int iVal) { m_iNestPriority = iVal; }

public:
	// the nesting priority
	int m_iNestPriority;
};

END_CUTLEADER_NAMESPACE()
