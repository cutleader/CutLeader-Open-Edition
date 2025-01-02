#pragma once

#include "clPartLayoutUICommon.h"
#include "XTreeCtrl.h"

DECLARE_CUTLEADER_CLASS(PartInstanceTopologyItemList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

class ClPartLayoutUI_Export PartTopologyTree : public XTreeCtrl
{
	DECLARE_DYNAMIC(PartTopologyTree)

public:
	PartTopologyTree(void);
	virtual ~PartTopologyTree(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpcreateStruct);

	DECLARE_MESSAGE_MAP()

public:
	void DisplayPartTop(PartInstanceTopologyItemListPtr pPartInstanceTopologyItemList);

	PartInstanceListPtr GetSelPartInstanceList();

private:
	void AppendTopNodes(HTREEITEM hParentNode, PartInstanceTopologyItemListPtr pPartInstanceTopologyItemList);

private:
	PartInstanceTopologyItemListPtr m_pPartInstanceTopologyItemList;
};

END_CUTLEADER_NAMESPACE()
