#pragma once

#include "clBaseDataCommon.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(IObserver)


BEGIN_CUTLEADER_NAMESPACE()


// the observer list.
class ClBaseData_Export ObserverList : public std::vector<IObserver*>
{
public:
	ObserverList(void);
	~ObserverList(void);

public:
	void UpdateAllObserver(int iEventID, const CComVariant& varEventContent);

	IObserver* GetObserverByID(long long iID);
};

END_CUTLEADER_NAMESPACE()
