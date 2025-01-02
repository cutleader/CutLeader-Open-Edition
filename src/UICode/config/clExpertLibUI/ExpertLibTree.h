#pragma once

#include "clExpertLibUICommon.h"
#include "XTreeCtrl.h"

BEGIN_CUTLEADER_NAMESPACE()

class IExpertLibNode;
typedef boost::shared_ptr<IExpertLibNode> ExpLibNodePtr;

// this tree will display the structure of the expert library.
// notes:
//   1) do not display the root node and the scheme items under part/sheet scheme, so the tree only have 3 layers and also do not need the recursive function to init the tree.
//   2) when "expert library item" node or "basic config item" node were selected, display the machine info of current expert library item.
//   3) when "schemes" item was selected, display first scheme item of the first scheme.
class ClExpertLibUI_Export ExpertLibTree : public XTreeCtrl
{
	DECLARE_DYNAMIC(ExpertLibTree)

public:
	ExpertLibTree(void);
	~ExpertLibTree(void);

public:
	// init the tree.
	void Init(ExpLibNodePtr pExpLibRootNode, LONGLONG iInitialParamConfigID);

	// 挂上一个专家库节点。
	// 注：
	//  1) iInitialParamConfigID为有效值时，会选中这个加工参数节点。
	HTREEITEM AppendExpLibItemNode(ExpLibNodePtr pExpLibItemNode, LONGLONG iInitialParamConfigID);

private:
	CImageList* m_pImageList;
};

END_CUTLEADER_NAMESPACE()
