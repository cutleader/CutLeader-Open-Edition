#pragma once

#include "clUILibCommon.h"
#include "clOptionCommon.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

// ���뵼�����á�
class ClOption_Export ImpExpConfig : public ClData
{
public:
	ImpExpConfig(void);
	~ImpExpConfig(void);

public: // get/set functions.
	void SetImpUnitType(DIM_UNIT_TYPE impUnit) { m_impUnit = impUnit; }
	DIM_UNIT_TYPE GetImpUnitType() const { return m_impUnit; }

	void SetImpTol(double dImpTol) { m_dImpTol = dImpTol; }
	double GetImpTol() const { return m_dImpTol; }

	void SetExpUnitType(DIM_UNIT_TYPE expUnit) { m_expUnit = expUnit; }
	DIM_UNIT_TYPE GetExpUnitType() const { return m_expUnit; }

	void SetLoopFilterSize(double dLoopFilterSize) { m_dLoopFilterSize = dLoopFilterSize; }
	double GetLoopFilterSize() const { return m_dLoopFilterSize; }

	void SetPatFilterSize(double dPatFilterSize) { m_dPatFilterSize = dPatFilterSize; }
	double GetPatFilterSize() const { return m_dPatFilterSize; }

	void SetOptimizeLimit(UINT iOptimizeLimit) { m_iOptimizeLimit = iOptimizeLimit; }
	UINT GetOptimizeLimit() const { return m_iOptimizeLimit; }

	void IsMoreArc(BOOL bMoreArc) { m_bMoreArc = bMoreArc; }
	BOOL IsMoreArc() const { return m_bMoreArc; }

	void SetAllowOffset(double dAllowOffset) { m_dAllowOffset = dAllowOffset; }
	double GetAllowOffset() const { return m_dAllowOffset; }

	BOOL IsProcessStdGrid() const { return m_bProcessStdGrid; }
	void IsProcessStdGrid(BOOL bProcessStdGrid) { m_bProcessStdGrid = bProcessStdGrid; }

	void SetAssignLimit(UINT iAssignLimit) { m_iAssignLimit = iAssignLimit; }
	UINT GetAssignLimit() const { return m_iAssignLimit; }

	void SetMergeItemCount(int iMergeItemCount) { m_iMergeItemCount = iMergeItemCount; }
	int GetMergeItemCount() const { return m_iMergeItemCount; }

	void SetMergeLineDis(double dMergeLineDis) { m_dMergeLineDis = dMergeLineDis; }
	double GetMergeLineDis() const { return m_dMergeLineDis; }

	void SetExpDxfVer(DXF_VER_TYPE iExpDxfVer) { m_iExpDxfVer = iExpDxfVer; }
	DXF_VER_TYPE GetExpDxfVer() const { return m_iExpDxfVer; }

	void IsExpShtBorder(BOOL bExpShtBorder) { m_bExpShtBorder = bExpShtBorder; }
	BOOL IsExpShtBorder() const { return m_bExpShtBorder; }

	void SetShtRptTplID(LONGLONG iShtRptTplID) { m_iShtRptTplID = iShtRptTplID; }
	LONGLONG GetShtRptTplID() const { return m_iShtRptTplID; }

	void SetNestRptTplID(LONGLONG iNestRptTplID) { m_iNestRptTplID = iNestRptTplID; }
	LONGLONG GetNestRptTplID() const { return m_iNestRptTplID; }

	void SetLabelRptTplID(LONGLONG iLabelRptTplID) { m_iLabelRptTplID = iLabelRptTplID; }
	LONGLONG GetLabelRptTplID() const { return m_iLabelRptTplID; }

	void SetStartRow(int iStartRow) { m_iStartRow = iStartRow; }
	int GetStartRow() { return m_iStartRow; }

	void SetPartNameCol(int iPartNameCol) { m_iPartNameCol = iPartNameCol; }
	int GetPartNameCol() const { return m_iPartNameCol; }

	void SetPartWidthCol(int iPartWidthCol) { m_iPartWidthCol = iPartWidthCol; }
	int GetPartWidthCol() const { return m_iPartWidthCol; }

	void SetPartHeightCol(int iPartHeightCol) { m_iPartHeightCol = iPartHeightCol; }
	int GetPartHeightCol() const { return m_iPartHeightCol; }

	void SetPartCountCol(int iPartCountCol) { m_iPartCountCol = iPartCountCol; }
	int GetPartCountCol() const { return m_iPartCountCol; }

	void SetPartPathCol(int iPartPathCol) { m_iPartPathCol = iPartPathCol; }
	int GetPartPathCol() const { return m_iPartPathCol; }

	void IsLoadPartFromPath(BOOL bLoadPartFromPath) { m_bLoadPartFromPath = bLoadPartFromPath; }
	BOOL IsLoadPartFromPath() const { return m_bLoadPartFromPath; }

private:
	/************************************************************************/
	// ����ѡ�

	// the import unit.
	DIM_UNIT_TYPE m_impUnit;

	// the import tolerance.
	double m_dImpTol;

	// ɾ���������Χ�ڵĻ�·��
	double m_dLoopFilterSize;

	// ����ͼ���Ż���
	UINT m_iOptimizeLimit;
	BOOL m_bMoreArc; // �Ƿ�����Բ�����
	double m_dAllowOffset;
	double m_dPatFilterSize;

	// whether process standard/grid pattern.
	BOOL m_bProcessStdGrid;

	// if the imported item count exceed this value, do not assign micro joint/Corner.
	UINT m_iAssignLimit;

	// ����ͼ�κϲ���
	int m_iMergeItemCount; // ��������Ԫ�ظ���С�����ֵʱ��Ҳִ�жԶ���ߵĺϲ���
	double m_dMergeLineDis; // ���ص���ƽ����֮��ľ���С�����ֵʱ���ϲ�֮��
	/************************************************************************/

	/************************************************************************/
	// ����ѡ�

	// the export unit.
	DIM_UNIT_TYPE m_expUnit;

	// ����dxf/dwg�汾��
	DXF_VER_TYPE m_iExpDxfVer;

	// �Ƿ񵼳���ı߿�
	BOOL m_bExpShtBorder;
	/************************************************************************/

	// Ĭ�ϱ���ģ�塣
	LONGLONG m_iShtRptTplID;
	LONGLONG m_iNestRptTplID;
	LONGLONG m_iLabelRptTplID;

	/************************************************************************/
	// ��Excel����������ʱ�����á�
	// ע��
	//  1) ��Щ������1��ʼ��

	// ����һ�п�ʼ��ȡ���ݡ�
	int m_iStartRow;

	// �е�ӳ���ϵ��
	int m_iPartNameCol;
	int m_iPartWidthCol;
	int m_iPartHeightCol;
	int m_iPartCountCol;
	int m_iPartPathCol;

	// �Ƿ�����·�����������
	BOOL m_bLoadPartFromPath;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
