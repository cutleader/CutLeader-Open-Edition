#pragma once

#include "ClSheetCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(SheetList)

BEGIN_CUTLEADER_NAMESPACE()

// 板材集合。
// 注：
//  1）用这个集合的地方会确定集合中的板材名称能不能重复。
class ClSheet_Export SheetList : public std::vector<SheetPtr>
{
public:
	SheetList(void);
	~SheetList(void);

public:
	SheetPtr GetSheetByID(LONGLONG iSheetID) const;

    // 得到板材中该零件的排版数量。
    // 注：
    //  1) 这个函数考虑了板材的数量。
    unsigned int GetPartNestedCount(LONGLONG iPartID) const;

    // 删除板材，这个函数不改变其他板材在集合中的顺序。
    void DeleteSheetByID(LONGLONG iSheetID);

    // 根据名称得到板材。
    SheetListPtr GetSheetsByName(CString strName);
};

END_CUTLEADER_NAMESPACE()
