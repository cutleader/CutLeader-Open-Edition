#pragma once

#include "BorderTplEntityBase.h"

DECLARE_CUTLEADER_CLASS(LabelTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

// 标签。
// 注：
//  1) 标签会在页上面以阵列方式打印。
//  2) 一个“标签模板页”上面只能有一个标签，其他类型的模板页不能放标签。
//  3) 在标签矩形框内的其他对象被认为在标签上面。
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
	// 行列数。
	int m_iRowNum;
	int m_iColumnNum;

	// 行列间距。
	int m_iRowGap;
	int m_iColumnGap;
};

END_CUTLEADER_NAMESPACE()
