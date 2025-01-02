#pragma once

#include "IData.h"

BEGIN_CUTLEADER_NAMESPACE()

// 观察者接口
// 注:
//    1) 观察者对象会被注册到主题对象中，一旦主题对象变化，即通知观察者对象。
//    2) 观察者对象可以知道主题对象的具体类型，而主题对象不应该知道观察者对象的细节。
class ClBaseData_Export IObserver
{
public:
    IObserver();
    virtual ~IObserver() {}

public:
    // 来自主题对象的更新通知。
    // 注:
    //  1) iEventID: 主题对象通过这个参数告诉观察者对象发生了什么事情。有的情况下观察者可能不关心事件的细节，那事件ID就可以设为-1。
    //  2) varEventContent: 事件的具体内容，可以不设置该值。
	virtual void Update(int iEventID, const CComVariant& varEventContent) = 0;

public:
    LONGLONG GetID() const { return m_iID; }

private:
    LONGLONG m_iID;
};

END_CUTLEADER_NAMESPACE()
