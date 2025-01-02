#pragma once

#include "clBaseDataCommon.h"
#include "DataProperty.h"
#include "DataPropItem.h"

DECLARE_CUTLEADER_CLASS(IData)

BEGIN_CUTLEADER_NAMESPACE()

// this is the interface for all cutleader data, cutleader数据接口。
// 注:
// 1) about "IsModified":
//    a) if new an object, we should remember to call "SetModified(TRUE)".
//    b) after load object from database or save the object to db, remember to call "SetModified(FALSE)".
//    c) for other cases, if user call Set***()/Update***()/... functions, user should call "SetModified(TRUE)", so user should determine when we should call this.
//    d) you cannot check the modified status only from "m_bModified", e.g.
//       i) for PartItem object, if a Line Pattern is added, "PartItem::m_bModified" will not changed, so we also need to check the involved object.
//       ii) for PatternLoopNode object, we do not check "PatternLoopNode::m_patPtr", because if we changed the color of "PatternLoopNode::m_patPtr",
//           we do not think PatternLoopNode is modified. so we should check the "composed" object, NOT "referenced" object. or you can have another criteria,
//           the "composed" object will be saved with "parent" object, BUT the "referenced" object will not.
// 2) which data will inherit from "IData"?
//    a) the data will be saved into database.
//    b) ID is useful for the data.
class ClBaseData_Export IData
{
public:
	virtual ~IData() {}

public:
	virtual BOOL IsModified() const = 0;
	virtual void SetModified(BOOL bModified) = 0; // 设置为“已修改”，并记录修改时间。
    virtual LONGLONG GetLatestModifyTimeStamp() const = 0; // 得到“最新修改时间”

	// compare the object ID.
	virtual BOOL IDEqual(LONGLONG iID) const = 0;

	// the properties of the data.
	virtual DataProperty GetProperty() const = 0;
	virtual void UpdateProperty(const DataPropItem& propItem) = 0;

	// 克隆数据对象。
	// 注：
	//   1) 有时数据对象的克隆函数只能完成部分工作，其他处理需要在外面完成。
	virtual IDataPtr Clone() const = 0;

public: // 这些函数不需要子类重载，所以没必要定义为虚函数，这样可以提高调用运行效率。
	LONGLONG GetID() const { return m_iID; }
	void SetID(LONGLONG iID) { m_iID = iID; }

	void SetParentID(LONGLONG iParentID) { m_iParentID = iParentID; }
	LONGLONG GetParentID() const { return m_iParentID; }

protected:
	// the unique ID of the object.
	// notes:
	//   1) the legal ID range is in [0, 2^63-1], this is the position section of long long type.
	//   2) for invalid ID, -1 will be saved.
	LONGLONG m_iID;

	// the parent ID of the object.
	// notes:
	//   1) actually not all data inherit from this class should set this variable. BUT we should set this variable in such cases,
	//      e.g., Pattern/PartCadData, PartCadData/Part...
	//   2) programmer MUST comment in the class if the class will set this variable.
	LONGLONG m_iParentID;

	// whether this object has been modified.
	BOOL m_bModified;

	// 最新修改时间。
	// 注：
	// 1）这个变量和BusinessData中的不一样，这个变量覆盖到所有ClData的每次修改。只要设置ClData的“已修改”标记时，就要更新这个值。
	//    从打开文档到关闭文档这个时间范围内，m_bModified可能会不断被设置为true和false，只有设为true时才更新这个时间戳。
	// 2) 这个时间戳会一直是一个有意义的值。
	// 3) 为了支持时间间隔很近的修改，这个时间戳必须是毫秒级的。
	LONGLONG m_iLatestModifyTimeStamp;
};

END_CUTLEADER_NAMESPACE()
