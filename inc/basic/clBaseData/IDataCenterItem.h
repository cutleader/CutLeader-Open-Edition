#pragma once

#include "clBaseDataCommon.h"
#include "IData.h"

BEGIN_CUTLEADER_NAMESPACE()

// “数据中心元素”接口。
// 注意：
//  1) we should init parent ID of the node.
class ClBaseData_Export IDataCenterItem : virtual public IData
{
public:
	IDataCenterItem() {}
	virtual ~IDataCenterItem() {}

public:
	virtual CString GetName() const = 0;
	virtual void SetName(CString strName) = 0;

	virtual void SetComment(CString strComment) = 0;
	virtual CString GetComment() const = 0;

	// whether this ProductItem is a folder.
	virtual BOOL IsFolder() const = 0;

	// get the path of the item.
	virtual CString GetItemPath() const = 0; // 不是全路径且不是斜杠结尾。
	virtual void SetItemPath(CString strPath) = 0;
	virtual CString GetFullPath() const = 0;

	virtual void SetCreateTime(LONGLONG iCreateTimeStamp) = 0;
	virtual LONGLONG GetCreateTime() const = 0;

	virtual void SetModifyTime(LONGLONG iModifyTimeStamp) = 0;
	virtual LONGLONG GetModifyTime() const = 0;
};

END_CUTLEADER_NAMESPACE()
