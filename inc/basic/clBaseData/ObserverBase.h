#pragma once

#include "IObserver.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

// the basic implementation for IObserver.
// ע��
// 1��֮ǰ�Ǵ�IObserver����̳У���������û�б�Ҫ��ȥ���ˡ����������Ҫ�ָ�����Ҫע��Ի���Ӹ���̳�ʱ��
//    �Ի����е���Ϣ����������ʾ4407��warning�����Բο�SubjectBase�еĽ��������
class ClBaseData_Export ObserverBase : public IObserver
{
public:
	ObserverBase();
	virtual ~ObserverBase();
};

END_CUTLEADER_NAMESPACE()
