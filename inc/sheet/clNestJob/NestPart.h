#pragma once

#include "ClNestJobCommon.h"
#include "NestPriority.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(PartItem)

BEGIN_CUTLEADER_NAMESPACE()

// 待排零件。
// 注：
//  1) “待排零件”对象和零件一一对应。
//  2) 整个排版过程中这个对象只有一份而不会被克隆。
class ClNestJob_Export NestPart : public ClData
{
public:
	NestPart(void);
	NestPart(PartItemPtr pPartItem);

	~NestPart(void);

public: // set/get functions.
	void SetPartItem(PartItemPtr pPartItem) { m_pPartItem = pPartItem; }
	const PartItem* GetPartItem() const { return m_pPartItem.get(); }

	void SetPriority(NestPriority nestPriority) { m_nestPriority = nestPriority; }
	NestPriority GetPriority() const { return m_nestPriority; }

	void SetNestCount(int iNestCount) { m_iNestCount = iNestCount; }
	int GetNestCount() const { return m_iNestCount; }

	void SetRotStyle(PartRotateType iRotStyle) { m_iRotStyle = iRotStyle; }
	PartRotateType GetRotStyle() const { return m_iRotStyle; }

public:
	// 根据旋转角度类型得到其名称。
	static CString GetName_ofPartRotateStyle(PartRotateType emPartRotate);

	// 根据旋转名称得到旋转角度类型。
	static PartRotateType GetPartRotateStyle_ofName(CString strRotateName);

	// 得到所有可用的旋转方式。
	static std::vector<CString> GetAllRotateStyles();

private:
	PartItemPtr m_pPartItem;

	// the nesting priority
	NestPriority m_nestPriority;

	// the count
	int m_iNestCount;

	// the rotation style of the part.
	PartRotateType m_iRotStyle;
};

END_CUTLEADER_NAMESPACE()
