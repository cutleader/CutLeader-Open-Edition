#pragma once

#include "clPartLayoutCommon.h"
#include "Point2D.h"
#include "Rect2D.h"
#include "ClData.h"
#include <vector>
#include <map>

DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// �����ʵ�������ϡ�
class ClPartLayout_Export PartInstanceList : public std::vector<PartInstancePtr>, public ClData
{
public:
	PartInstanceList(void);
	virtual ~PartInstanceList(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;

	// clone the part inst list.
	// notes:
	//   1) do not clone part placement within each part inst.
	virtual IDataPtr Clone() const override;

public:
	// add the part instance by the part placement
	// notes: 
	//   1) when we add a part placement into sheet, we should also add the relative part instances.
	void AddPartInstanceByPartPlacement(PartPlacementPtr pPartPlacement);

	void AppendPartInstances(const PartInstanceList* pPartInstList) { this->insert(this->end(), pPartInstList->begin(), pPartInstList->end()); }


	/************************************************************************/
	// ����ɾ�����ʵ����

	// delete the part instance by the part placement
	// notes: 
	//   1) when we delete a part placement from sheet, we should also delete the relative part instances.
	void DeletePartInstanceByPartPlacement(const PartPlacement* pPartPlacement);

	// delete the part instance by ID.
	void DeletePartInstanceByID(LONGLONG iId);

	// ɾ����������ʵ������
	void DeletePartInstanceList(const PartInstanceList* pPartInstances);

	// �������IDɾ�����ʵ����
	void DeletePartInstanceByPartID(LONGLONG iPartID);
	/************************************************************************/


	/************************************************************************/
	// about getting part instances.

	// get the part instance by id
	PartInstancePtr GetPartInstanceByID(LONGLONG iId) const;

	// get the index of "pPartInstance".
	int GetPartInstanceIndex(const PartInstance* pPartInstance) const;

	// get the part instance by the grid part placement and the row/column.
	// notes: 
	//   1) you know, for grid, part pmt and the row/column can decide a part, so only call this function for grid.
	PartInstancePtr GetGridPartInstance(const PartPlacement* pPartPlacement, int iRow, int iCol) const;

	// get part insts by the part pmt.
	PartInstanceListPtr GetPartInstanceByPartPlacement(const PartPlacement* pPartPlacement) const;

	// �õ����������ġ����ʵ������
	/* ����:
	*  partRectMap: ���ʵ���и�����·ʵ������Ӿ��Ρ�
	*/
	// ע��
	//   1) �Ƚϵ������ǵ㵽����Χ��·ʵ�����ľ��롣
	PartInstancePtr GetClosestPartInstance(const Point2D& pt, const std::map<LONGLONG, std::pair<Rect2D, std::map<LONGLONG, Rect2D>>>& partRectMap);

	// �������ID�õ������ʵ�������ϡ�
	PartInstanceListPtr GetPartInstances(LONGLONG iPartID) const;

	// �õ��С���Χ�����������ʵ����
	PartInstanceListPtr GetPartInstancesHasOutLoop() const;

	// �õ����С��ֵ��������
	PartInstanceListPtr GetPartInstances_brother(PartInstancePtr pPartInstance) const;
	/************************************************************************/


	// whether the pattern loop exist.
	BOOL PatternLoopExist(LONGLONG iPatternLoopID);

	// �������ʵ���и�����·ʵ������Ӿ��Ρ�
	// ע��
	//  1) ���ŵġ���·ʵ����������
	//  2) �����л�·ʵ������õ������Ӿ��Ρ�
	//  3) ��pair.first���Ƿŵġ���Χ��·ʵ��������Ӿ��Ρ�
	//  4) ����û�С���Χ��·�������ʵ��������Χ��·ʵ��������Ӿ����õ�������ͼ�ε���Ӿ��Ρ�
	std::map<LONGLONG, std::pair<Rect2D, std::map<LONGLONG, Rect2D> > > GetLoopInstanceRect();
};

END_CUTLEADER_NAMESPACE()
