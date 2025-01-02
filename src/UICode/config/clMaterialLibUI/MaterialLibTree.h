#pragma once

#include "clMaterialLibUICommon.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(IMaterialLibNode)

BEGIN_CUTLEADER_NAMESPACE()

// this tree will display the structure of the material library.
// notes:
//   1) do not display the root node.
class ClMaterialLibUI_Export MaterialLibTree : public SkinXTreeCtrl
{
	DECLARE_DYNAMIC(MaterialLibTree)

public:
	MaterialLibTree(void);
	~MaterialLibTree(void);

public:
	// init the tree.
	void Init(IMaterialLibNodePtr pMatLibNode);

	// 挂上材料节点和其子孙节点。
	HTREEITEM AppendMatNode(IMaterialLibNodePtr pMatNode);

private:
	// init the sub nodes of matType node.
	void InitMatTypeNode(IMaterialLibNodePtr pMatNode, HTREEITEM hMatTypeNode);

	// init the sub nodes of matName node.
	void InitMatNameNode(IMaterialLibNodePtr pMatThickNode, HTREEITEM hMatNameNode);

private:
	// the icons used on the tree.
	CImageList* m_pImageList;
};

END_CUTLEADER_NAMESPACE()
