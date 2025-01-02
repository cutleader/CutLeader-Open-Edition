#pragma once

#include "clPartLayoutCommon.h"
#include "Point2D.h"
#include "Rect2D.h"
#include "ClData.h"
#include "SubjectBase.h"
#include <vector>
#include <map>

DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(LoopCornerDataList)
DECLARE_CUTLEADER_CLASS(LoopMicroJointList)
DECLARE_CUTLEADER_CLASS(PartItemList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartList)

BEGIN_CUTLEADER_NAMESPACE()

// ��������á����ϡ�
class ClPartLayout_Export PartPlacementList : public std::vector<PartPlacementPtr>, public ClData, public SubjectBase
{
public:
	PartPlacementList(void);
	virtual ~PartPlacementList(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;

	// clone the part placement list.
	// notes:
	//   1) do not clone part BUT clone loop pierce data.
	//   2) do not clone the draw data, user should update it outside.
	virtual IDataPtr Clone() const override;

public:
	/************************************************************************/
	// about finding the part placement

	// get the part placement by id
	PartPlacementPtr GetPartPlacementByID(LONGLONG iId) const;

	// get the part placement which is closest to "pt".
	PartPlacementPtr GetPartPlacementClosestTo(const Point2D& pt) const;
	PartPlacementPtr GetPartPlacementClosestTo(const Point2D& pt, const std::map<LONGLONG, Rect2D>& m_rects_of_partPmt) const;

	// get the part placements within "rect".
	PartPlacementListPtr GetPartPlacementInRect(const Rect2D& rect) const;
    PartPlacementListPtr GetPartPlacementInRect(const Rect2D& rect, const std::map<LONGLONG, Rect2D>& m_rects_of_partPmt) const;

	// get the part placements which are from "iPartID".
	PartPlacementListPtr GetPartPlacementByPart(LONGLONG iPartID) const;

	// get index of "pPartPlacement".
	int GetPartPlacementIndex(const PartPlacement* pPartPlacement) const;
	/************************************************************************/


	/************************************************************************/
	// ���ڵõ��������Ϣ��

	// �õ��������õ�������ġ�CAD���ݡ���
	// ע��
	//  1) �õ����ID��CAD���ݵĶ�Ӧ�����ID���ظ���
	std::map<LONGLONG, PartCadDataPtr> GetAllPartCadData() const;

	// �õ�����е�����
	PartListPtr GetPartList() const;

	// �õ���������á����������õ������в�ͬ�����
	IntegerListPtr GetPartIDList() const;

	// �õ����ʵ���ĸ�����
	int GetPartInstanceCount(LONGLONG iPartID) const;
	int GetPartInstanceCount() const;
	/************************************************************************/


	/************************************************************************/
	// ����ɾ��������á�

	// ɾ��һ����������á���
	void RemovePartPlacement(const PartPlacement* partPmtPtr);

	// ɾ�������������á���
	void DeletePartPlacementList(const PartPlacementList* pPartPlacements);

	// �������IDɾ��������á�
	void DeletePartPmtByPartID(LONGLONG iPartID);
	/************************************************************************/


	// get the base geometry.
	LineArc2DListPtr Calculate2DLineArcs() const;

	// �õ����С���·΢������������
	LoopMicroJointListPtr GetAllLoopMicroJointList() const;

	// �õ����С���·����������
	LoopCornerDataListPtr GetAllLoopCornerList() const;

	// �õ���������á����ϵ���Χ���Ρ�
	// ע��
	//  1) �������ӹ����ա�
	Rect2D GetRectBox() const;

	void AppendPartPmts(const PartPlacementList* pPartPlacements) { this->insert(this->end(), pPartPlacements->begin(), pPartPlacements->end()); }

	// �任������ü��ϡ�
	void Transform(const Matrix2D& transformMatrix);
};

END_CUTLEADER_NAMESPACE()
