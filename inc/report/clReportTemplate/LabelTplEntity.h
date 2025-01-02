#pragma once

#include "BorderTplEntityBase.h"

DECLARE_CUTLEADER_CLASS(LabelTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

// ��ǩ��
// ע��
//  1) ��ǩ����ҳ���������з�ʽ��ӡ��
//  2) һ������ǩģ��ҳ������ֻ����һ����ǩ���������͵�ģ��ҳ���ܷű�ǩ��
//  3) �ڱ�ǩ���ο��ڵ�����������Ϊ�ڱ�ǩ���档
class ClReportTemplate_Export LabelTplEntity : public BorderTplEntityBase
{
public:
	LabelTplEntity(void);
	~LabelTplEntity(void);

public: // functions from IData interface.
	virtual IDataPtr Clone() const override;

public: // implement ITplEntity interface.
	virtual ENTITY_TYPE GetType() const { return ENTITY_LABEL; }

public:
	void SetRowNum(int iRowNum) { m_iRowNum = iRowNum; }
	int GetRowNum() { return m_iRowNum; }

	void SetColumnNum(int iColumnNum) { m_iColumnNum = iColumnNum; }
	int GetColumnNum() { return m_iColumnNum; }

	void SetRowGap(int iRowGap) { m_iRowGap = iRowGap; }
	int GetRowGap() { return m_iRowGap; }

	void SetColumnGap(int iColumnGap) { m_iColumnGap = iColumnGap; }
	int GetColumnGap() { return m_iColumnGap; }

private:
	// ��������
	int m_iRowNum;
	int m_iColumnNum;

	// ���м�ࡣ
	int m_iRowGap;
	int m_iColumnGap;
};

END_CUTLEADER_NAMESPACE()
