#pragma once

#include "ClNestJobCommon.h"
#include "NestPriority.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(PartItem)

BEGIN_CUTLEADER_NAMESPACE()

// ���������
// ע��
//  1) �������������������һһ��Ӧ��
//  2) �����Ű�������������ֻ��һ�ݶ����ᱻ��¡��
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
	// ������ת�Ƕ����͵õ������ơ�
	static CString GetName_ofPartRotateStyle(PartRotateType emPartRotate);

	// ������ת���Ƶõ���ת�Ƕ����͡�
	static PartRotateType GetPartRotateStyle_ofName(CString strRotateName);

	// �õ����п��õ���ת��ʽ��
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
