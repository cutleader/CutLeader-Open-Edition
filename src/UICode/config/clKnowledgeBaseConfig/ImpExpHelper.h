#pragma once

#include "cutLeaderCommon.h"
#include "ExpertLibTree.h"
#include "MaterialLibTree.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

typedef boost::shared_ptr<std::vector<CString>> StringListPtr;

// all import/export action will be delegated to this class.
class ImpExpHelper
{
public:
	// export ExpertLibItem.
	// notes:
	//   1) now we can not export the reserved one.
	static void ExportExpLibItem(ExpertLibTree* pExpLibTree);

	// import ExpertLibItem.
	static void ImportExpLibItem(ExpertLibTree* pExpLibTree);

	// 导出材料。
	static void ExportMat(MaterialLibTree* pMatLibTree);

	// 导入材料。
	static void ImportMat(MaterialLibTree* pMatLibTree);

private:
	// check whether "strName" existed in "pExistNameList".
	// notes:
	//   1) if "strName" existed within "pExistNameList",
	//      we should change the name of "strName", the rule is adding "_n" postfix,
	//      then return the new name with "strNewName".
	//   2) if "strName" do not exist within "pExistNameList", return TRUE.
	static BOOL CheckName(StringListPtr pExistNameList, CString strName, CString& strNewName);
};

END_CUTLEADER_NAMESPACE()
