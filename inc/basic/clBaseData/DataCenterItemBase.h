#pragma once

#include "clBaseDataCommon.h"
#include "ClData.h"
#include "IDataCenterItem.h"

BEGIN_CUTLEADER_NAMESPACE()

// the basic implementation for data/folder node.
class ClBaseData_Export DataCenterItemBase : public IDataCenterItem,
											 virtual public ClData
{
public:
	DataCenterItemBase(void);
	virtual ~DataCenterItemBase(void);

public: // get/set functions.
	virtual CString GetName() const override { return m_strName; }
	virtual void SetName(CString strName) override { m_strName = strName; }

	virtual void SetComment(CString strComment) override { m_strComment = strComment; }
	virtual CString GetComment() const override { return m_strComment; }

	virtual CString GetItemPath() const override { return m_strPath; }
	virtual void SetItemPath(CString strPath) override { m_strPath = strPath; }

	void SetCreateTime(LONGLONG iCreateTimeStamp) override { m_iCreateTimeStamp = iCreateTimeStamp; }
	LONGLONG GetCreateTime() const override { return m_iCreateTimeStamp; }

	void SetModifyTime(LONGLONG iModifyTimeStamp) override { m_iModifyTimeStamp = iModifyTimeStamp; }
	LONGLONG GetModifyTime() const override { return m_iModifyTimeStamp; }

public:
	// get the full path of the product item.
	CString GetFullPath() const;

protected:
	// name of the node.
	CString m_strName;

	CString m_strComment;

	// the path of the node.
	// notes: 
	//   1) the path do not include the name of this item. e.g., for "folder1" under Workspace folder, "m_strPath" 
	//      will be "My Workspace".
	CString m_strPath;

	// create and modify date.
	LONGLONG m_iCreateTimeStamp;
	LONGLONG m_iModifyTimeStamp;
};

END_CUTLEADER_NAMESPACE()
