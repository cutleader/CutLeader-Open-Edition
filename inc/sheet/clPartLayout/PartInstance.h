#pragma once

#include "clPartLayoutCommon.h"
#include "ClData.h"
#include "Matrix2D.h"
#include "Rect2D.h"
#include <map>

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(PartInstance)

BEGIN_CUTLEADER_NAMESPACE()

// �����ʵ������
class ClPartLayout_Export PartInstance : public ClData
{
public:
	PartInstance(PartPlacementPtr pPartPlacement);
	virtual ~PartInstance(void);

public: // implement IData interface.
	// clone the part inst.
	// notes:
	//   1) do not clone part placement.
	virtual IDataPtr Clone() const override;

public: // get/set functions
	void SetPartPlacement(PartPlacementPtr pPartPlacement) { m_pPartPlacement = pPartPlacement; }
	PartPlacementPtr GetPartPlacement() const { return m_pPartPlacement; }

	void SetRow(int iRow) { m_iRow = iRow; }
	int GetRow() const { return m_iRow; }

	void SetColumn(int iColumn) { m_iColumn = iColumn; }
	int GetColumn() const { return m_iColumn; }

public:
	// whether the pattern loop exist.
	BOOL PatternLoopExist(LONGLONG iPatternLoopID) const;

	// get the transform matrix which can transform part instance to sheet mode.
	Matrix2D GetTransformMatrix() const;

	// �������ʵ���и�����·ʵ������Ӿ��Ρ�
	// ע��
	//  1) ���ŵġ���·ʵ����������
	//  2) �����л�·ʵ������õ������Ӿ��Ρ�
	//  3) ��pair.first���Ƿŵġ���Χ��·ʵ��������Ӿ��Ρ�
	//  4) ��Ӧ��·��IDҲ���ء�
	//  5) ����û�С���Χ��·�������ʵ��������Χ��·ʵ��������Ӿ����õ�������ͼ�ε���Ӿ��Ρ�
	std::pair<Rect2D, std::map<LONGLONG, Rect2D> > GetLoopInstanceRect() const;

	// ���ʵ������ľ��롣
	// ע��
	//  1) �����С���Χ��·�������ʵ��������㵽����Χ��·ʵ�����ľ��롣
	//  2) ����û�С���Χ��·�������ʵ��������㵽�����л���ͼ�Ρ��ľ��롣
	double DistanceToPt(const Point2D& pt) const;

	// calculate the 2d line/arcs of part placement.
	// notes: 
	//  1) do not calculate cut feature.
	LineArc2DListPtr Calculate2DLineArcs() const;

	// �õ����ʵ�������ĵ㡣
	// ע��
	//  1) Ҫ�ǡ����ʵ������Ӿ��ε����ĵ�������ϣ��򷵻�������ĵ㡣
	//     ���򷵻���������һ�㣬Ҫ��û����������������������һ�㡣
	//  2) ������ʵ��û�С���Χ����������������ͼ�ε����ĵ㡣
	Point2D GetCenterPt() const;

	// ���ʵ���Ƿ��С���Χ��������
	BOOL HasBoundaryLoop() const;

	// �õ����м������������˹�ϵ���������������Ϊ�˷��㣬���ǵ��ü���get�������ɡ�
	const LoopTopologyItemList* GetLoopTopologyItems() const;
	LoopTopologyItemListPtr GetLoopTopologyItems_FW() const;

private:
	// the part placement object.
	PartPlacementPtr m_pPartPlacement;

	// if the Part placement is grid, we should indicate the position of the part instance.
	int m_iRow;
	int m_iColumn;
};

END_CUTLEADER_NAMESPACE()
