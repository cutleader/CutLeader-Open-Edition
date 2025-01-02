#pragma once

#include "clBaseDataCommon.h"
#include "DataProperty.h"
#include "DataPropItem.h"

DECLARE_CUTLEADER_CLASS(IData)

BEGIN_CUTLEADER_NAMESPACE()

// this is the interface for all cutleader data, cutleader���ݽӿڡ�
// ע:
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
	virtual void SetModified(BOOL bModified) = 0; // ����Ϊ�����޸ġ�������¼�޸�ʱ�䡣
    virtual LONGLONG GetLatestModifyTimeStamp() const = 0; // �õ��������޸�ʱ�䡱

	// compare the object ID.
	virtual BOOL IDEqual(LONGLONG iID) const = 0;

	// the properties of the data.
	virtual DataProperty GetProperty() const = 0;
	virtual void UpdateProperty(const DataPropItem& propItem) = 0;

	// ��¡���ݶ���
	// ע��
	//   1) ��ʱ���ݶ���Ŀ�¡����ֻ����ɲ��ֹ���������������Ҫ��������ɡ�
	virtual IDataPtr Clone() const = 0;

public: // ��Щ��������Ҫ�������أ�����û��Ҫ����Ϊ�麯��������������ߵ�������Ч�ʡ�
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

	// �����޸�ʱ�䡣
	// ע��
	// 1�����������BusinessData�еĲ�һ��������������ǵ�����ClData��ÿ���޸ġ�ֻҪ����ClData�ġ����޸ġ����ʱ����Ҫ�������ֵ��
	//    �Ӵ��ĵ����ر��ĵ����ʱ�䷶Χ�ڣ�m_bModified���ܻ᲻�ϱ�����Ϊtrue��false��ֻ����Ϊtrueʱ�Ÿ������ʱ�����
	// 2) ���ʱ�����һֱ��һ���������ֵ��
	// 3) Ϊ��֧��ʱ�����ܽ����޸ģ����ʱ��������Ǻ��뼶�ġ�
	LONGLONG m_iLatestModifyTimeStamp;
};

END_CUTLEADER_NAMESPACE()
