#pragma once

#include "ISubject.h"

DECLARE_CUTLEADER_CLASS(ObserverList)

BEGIN_CUTLEADER_NAMESPACE()

// the basic implementation for interface ISubject.
class ClBaseData_Export SubjectBase : virtual public ISubject
{
public:
	SubjectBase();
	virtual ~SubjectBase();

public: // implement interface ISubject.
	virtual void AddObserver(IObserver* pObserver) override;
	virtual void RemoveObserver(LONGLONG iObserverID) override;
	virtual void NotifyAllObserver(int iEventID, const CComVariant& varEventContent) override;

private:
	// these observers will watch this subject.
	ObserverListPtr m_pObserverList;
};


// 第二个实现，和SubjectBase的区别就是没有从ISubject虚拟继承，有时子类需要这样的方式。特别是对话框从该类继承时，
// 如果该类使用虚拟继承，对话框中的消息处理函数会提示4407的warning。
class ClBaseData_Export SubjectBase_2 : public ISubject
{
public:
	SubjectBase_2();
	virtual ~SubjectBase_2();

public: // implement interface ISubject.
	virtual void AddObserver(IObserver* pObserver) override;
	virtual void RemoveObserver(LONGLONG iObserverID) override;
	virtual void NotifyAllObserver(int iEventID, const CComVariant& varEventContent) override;

private:
	// these observers will watch this subject.
	ObserverListPtr m_pObserverList;
};

END_CUTLEADER_NAMESPACE()
