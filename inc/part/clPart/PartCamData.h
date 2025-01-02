#pragma once

#include "clPartCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(LoopCutFeature)
DECLARE_CUTLEADER_CLASS(LoopCutFeatureList)
DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)
DECLARE_CUTLEADER_CLASS(LoopCornerDataList)
DECLARE_CUTLEADER_CLASS(LoopMicroJointList)
DECLARE_CUTLEADER_CLASS(LoopCondList)
DECLARE_CUTLEADER_CLASS(PartCamData)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// ���ӹ����ա���
// ע��
//  1) һ�׹������ݶ�Ӧһ�����������á���
class ClPart_Export PartCamData : public ClData
{
public:
	PartCamData();
	PartCamData(LONGLONG iParamConfigID);
	virtual ~PartCamData(void);

public: // implement IData interface
	// we should check:
	//  1) whether the scheme is changed.
	//  2) whether cam features are changed.
	virtual BOOL IsModified() const override;

    virtual LONGLONG GetLatestModifyTimeStamp() const override;

	// ע:
	//   1) ������������»��档
	IDataPtr Clone() const override;

public: // get/set functions.
	LoopCutFeatureListPtr GetLoopFeatureList() const { return m_pLoopFeatureList; }
	void SetLoopFeatureList(LoopCutFeatureListPtr pLoopFeatureList) { m_pLoopFeatureList = pLoopFeatureList; }

	void SetParamConfigID(LONGLONG iID) { m_iParamConfigID = iID; }
	LONGLONG GetParamConfigID() const { return m_iParamConfigID; }

public:
	// get the pierce features of all loops.
	LoopStartCutDataListPtr GetLoopStaEndCutList() const;

	// get the loop micro joint features of all loops.
	LoopMicroJointListPtr GetLoopMicroJointList() const;

	// get the loop corner features of all loops.
	LoopCornerDataListPtr GetLoopCornerList() const;

    // �õ���������loop feature��
    LoopCutFeaturePtr GetBoundaryLoopFeature(const LoopTopologyItemList* pLoopTopologyItems) const;

private:
	// the cam features of the loops.
	LoopCutFeatureListPtr m_pLoopFeatureList;

	// the scheme info used by this PartCamData.
	LONGLONG m_iParamConfigID;
};

END_CUTLEADER_NAMESPACE()
