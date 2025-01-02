#pragma once

#include "clSheetCommon.h"
#include "BusinessData.h"
#include "Point2D.h"
#include "boost/enable_shared_from_this.hpp"

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(SheetCache)
DECLARE_CUTLEADER_CLASS(PartGroupList)
DECLARE_CUTLEADER_CLASS(RefPtData)

BEGIN_CUTLEADER_NAMESPACE()

// ��Ķ���
// ע��
// 1) ������ǵ�ͼʱ��Ҫע��������ȷ�Ĳ��ϳߴ磬��ĸ���ֻ��Ϊ1�����ʵ������ֻ��Ϊ1����Ҫ�޸İ�ĵ����ơ�����ڱ�����ʱ����Щ������м�飬�Է�ֹ��С�����ô���
class ClSheet_Export Sheet : public ClData
{
public:
	Sheet(const CString& strName, bool bIsCanvas, RefPtDataPtr pRefPtData, LONGLONG iMaterialSizeID, LONGLONG iParamConfigID);
	virtual ~Sheet(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;

public: // get/set functions.
	void SetName(const CString& strName) { m_strName = strName; }
	const CString& GetName() const { return m_strName; }

	bool GetIsCanvas() const { return m_bIsCanvas; }

	PartPlacementListPtr GetPartPlacements() const { return m_pPartPmtList; }
	void SetPartPlacements(PartPlacementListPtr pPartPlacements) { m_pPartPmtList = pPartPlacements; }

	PartInstanceListPtr GetPartInstances() const { return m_pPartInstList; }
	void SetPartInsts(PartInstanceListPtr pPartInstances) { m_pPartInstList = pPartInstances; }

    PartGroupListPtr GetPartGroupList() const { return m_pPartGroupList; }
    void SetPartGroupList(PartGroupListPtr pPartGroupList) { m_pPartGroupList = pPartGroupList; }

	void SetMaterialSizeID(LONGLONG iMaterialSizeID) { m_iMaterialSizeID = iMaterialSizeID; }
	LONGLONG GetMaterialSizeID() const { return m_iMaterialSizeID; }

	RefPtDataPtr GetRefPtData() const { return m_pRefPtData; }

	CutSequenceListPtr GetCutSequences() const { return m_pCutSequences; }
	void SetCutSequences(CutSequenceListPtr pCutSequences) { m_pCutSequences = pCutSequences; }

	void SetParamConfigID(LONGLONG iParamConfigID) { m_iParamConfigID = iParamConfigID; }
	LONGLONG GetParamConfigID() const { return m_iParamConfigID; }

	void SetSheetCache(SheetCachePtr pSheetCache) { m_pSheetCache = pSheetCache; }
	SheetCachePtr GetSheetCache() const { return m_pSheetCache; }

	void SetPreviewData(ImgDataPtr pPreviewData) { m_pPreviewData = pPreviewData; }
	ImgDataPtr GetPreviewData() const { return m_pPreviewData; }

	unsigned int GetSheetCount() const { return m_iSheetCount; }
	void SetSheetCount(unsigned int iSheetCount) { m_iSheetCount = iSheetCount; }

public: // other functions.
	// calc the base geoms of the sheet.
	// notes: 
	//   1) only return the geometry data, do not include lead in/out, useless Region,....
	LineArc2DListPtr Calculate2DLineArcs() const;

	// get the inst count of the part in this sheet.
	int GetPartInstanceCount(LONGLONG iPartID) const;

	// �õ����������ʡ�
	double CalcMaterialUtilization() const;

private:
	CString m_strName;

	// �Ƿ��ǵ�ͼ��
	// ע��
	// 1) ֮ǰ��ͨ�����ʹ�õĲ������ж��ǲ��ǵ�ͼ���о����Ǽ������Ǻ��ѯ���������㣬��Ȼ��Ϣ�е����ࡣ
	bool m_bIsCanvas;


	/************************************************************************/
	// �������������ݡ�

	// all part placements in the sheet.
	PartPlacementListPtr m_pPartPmtList;

	// all part instances in the sheet.
	PartInstanceListPtr m_pPartInstList;

	// �������
	PartGroupListPtr m_pPartGroupList;
	/************************************************************************/


	/************************************************************************/
	// ����������ݡ�

	// �ο������ݡ�
	// ע��
	// 1) ������ĺ����ù���ʱ���ר�ҿ��ʼ��������ݡ�
	RefPtDataPtr m_pRefPtData;

	// ���򼯺ϡ�
	CutSequenceListPtr m_pCutSequences;
	/************************************************************************/


	/************************************************************************/
	// configuration info.

	// the material size which produce this sheet.
	// notes:
	//   1) after deleting the sheet, we should return the material size.
	//   2) should not be INVALID_ID.
	LONGLONG m_iMaterialSizeID;

	// the scheme info used by this sheet.
	LONGLONG m_iParamConfigID;
	/************************************************************************/


	// the cache data of sheet.
	SheetCachePtr m_pSheetCache;

	// ��ĵ�Ԥ��ͼƬ��
	// ע��
	// 1) �������ֻ�ڴ����ݿ���ذ��ʱ��ʼ��������ʵʱ���¡�
	// 2) �ڱ�����ʱ�������Ԥ�����ݴ������ݿ⡣
	ImgDataPtr m_pPreviewData;

	// ��ĸ���
	unsigned int m_iSheetCount;
};

END_CUTLEADER_NAMESPACE()
