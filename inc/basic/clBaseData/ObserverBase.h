#pragma once

#include "IObserver.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

// the basic implementation for IObserver.
// 注：
// 1）之前是从IObserver虚拟继承，后来发现没有必要就去掉了。如果后面需要恢复，需要注意对话框从该类继承时，
//    对话框中的消息处理函数会提示4407的warning。可以参考SubjectBase中的解决方法。
class ClBaseData_Export ObserverBase : public IObserver
{
public:
	ObserverBase();
	virtual ~ObserverBase();
};

END_CUTLEADER_NAMESPACE()
