#pragma once

#include "IData.h"

BEGIN_CUTLEADER_NAMESPACE()

// �۲��߽ӿ�
// ע:
//    1) �۲��߶���ᱻע�ᵽ��������У�һ���������仯����֪ͨ�۲��߶���
//    2) �۲��߶������֪���������ľ������ͣ����������Ӧ��֪���۲��߶����ϸ�ڡ�
class ClBaseData_Export IObserver
{
public:
    IObserver();
    virtual ~IObserver() {}

public:
    // �����������ĸ���֪ͨ��
    // ע:
    //  1) iEventID: �������ͨ������������߹۲��߶�������ʲô���顣�е�����¹۲��߿��ܲ������¼���ϸ�ڣ����¼�ID�Ϳ�����Ϊ-1��
    //  2) varEventContent: �¼��ľ������ݣ����Բ����ø�ֵ��
	virtual void Update(int iEventID, const CComVariant& varEventContent) = 0;

public:
    LONGLONG GetID() const { return m_iID; }

private:
    LONGLONG m_iID;
};

END_CUTLEADER_NAMESPACE()
