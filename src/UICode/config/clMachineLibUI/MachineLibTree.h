#pragma once

#include "clMachineLibUICommon.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(MachineLibNode)

BEGIN_CUTLEADER_NAMESPACE()

// this tree will display the structure of the mac library.
class ClMachineLibUI_Export MachineLibTree : public SkinXTreeCtrl
{
	DECLARE_DYNAMIC(MachineLibTree)

public:
	MachineLibTree(void);
	~MachineLibTree(void);

public:
	// init the tree.
	void Init(MachineLibNodePtr pMacLibRootNode);

	// append mac item node to the tree.
	HTREEITEM AppendExpLibItemNode(HTREEITEM hRootItem, MachineLibNodePtr pMacLibItemNode);

private:
	CImageList* m_pImageList;
};

END_CUTLEADER_NAMESPACE()
