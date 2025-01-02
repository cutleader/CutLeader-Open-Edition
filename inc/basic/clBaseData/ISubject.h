#pragma once

#include "clBaseDataCommon.h"

DECLARE_CUTLEADER_CLASS(IObserver)

BEGIN_CUTLEADER_NAMESPACE()

// the subject which can be "observed" by the observers.
class ClBaseData_Export ISubject
{
public:
    virtual ~ISubject() {}

public:
	// add an observer to the subject.
	// notes:
	//   1) here, we do not use smart pointer "ObserverPtr", because, for some classes(PartView/...), they are not created by such
	//      statement "PartViewPtr pPartView(new PartView(...))" and can not call "shared_from_this()" in them.
	//   2) anyway the row pointer is enough for this case.
	virtual void AddObserver(IObserver* pObserver) = 0;

	// remove the observer from the subject.
	virtual void RemoveObserver(LONGLONG iObserverID) = 0;

	// notify all observers to update.
	virtual void NotifyAllObserver(int iEventID, const CComVariant& varEventContent) = 0;
};

END_CUTLEADER_NAMESPACE()
