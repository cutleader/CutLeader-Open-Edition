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

// 板材对象。
// 注：
// 1) 当板材是底图时，要注意设置正确的材料尺寸，板材个数只能为1，零件实例数量只能为1，不要修改板材的名称。最好在保存板材时对这些规则进行检查，以防止不小心设置错误。
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

	// 得到材料利用率。
	double CalcMaterialUtilization() const;

private:
	CString m_strName;

	// 是否是底图。
	// 注：
	// 1) 之前是通过板材使用的材料来判断是不是底图，感觉还是加这个标记后查询起来更方便，虽然信息有点冗余。
	bool m_bIsCanvas;


	/************************************************************************/
	// 零件布局相关数据。

	// all part placements in the sheet.
	PartPlacementListPtr m_pPartPmtList;

	// all part instances in the sheet.
	PartInstanceListPtr m_pPartInstList;

	// 零件分组
	PartGroupListPtr m_pPartGroupList;
	/************************************************************************/


	/************************************************************************/
	// 工序相关数据。

	// 参考点数据。
	// 注：
	// 1) 创建板材和重置工序时会从专家库初始化这个数据。
	RefPtDataPtr m_pRefPtData;

	// 工序集合。
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

	// 板材的预览图片。
	// 注：
	// 1) 这个变量只在从数据库加载板材时初始化，不会实时更新。
	// 2) 在保存板材时，计算出预览数据存入数据库。
	ImgDataPtr m_pPreviewData;

	// 板材个数
	unsigned int m_iSheetCount;
};

END_CUTLEADER_NAMESPACE()
