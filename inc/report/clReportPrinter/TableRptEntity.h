#pragma once

#include "BorderRptEntityBase.h"

DECLARE_CUTLEADER_CLASS(TableColumnList)
DECLARE_CUTLEADER_CLASS(ColumnDataList)
DECLARE_CUTLEADER_CLASS(TableRptEntity)

BEGIN_CUTLEADER_NAMESPACE()

// the table entity in the report.
class TableRptEntity : public BorderRptEntityBase
{
public:
	TableRptEntity(void);
	~TableRptEntity(void);

public: // implement IRptEntity interface.
	virtual RPT_ENTITY_TYPE GetType() const { return RPT_ENTITY_TABLE; }

public: // get/set functions.
	/************************************************************************/
	// config for column lines.

	BOOL IsShowColumnLine() const { return m_bShowColumnLine; }
	void IsShowColumnLine(BOOL bShowColumnLine) { m_bShowColumnLine = bShowColumnLine; }

	double GetColumnLineThickness() const { return m_dColumnLineThickness; }
	void SetColumnLineThickness(double dColumnLineThickness) { m_dColumnLineThickness = dColumnLineThickness; }

	LINE_STYLE_TYPE GetColumnLineStyle() const { return m_columnLineStyle; }
	void SetColumnLineStyle(LINE_STYLE_TYPE columnLineStyle) { m_columnLineStyle = columnLineStyle; }

	COLORREF GetColumnLineColor() const { return m_columnLineColor; }
	void SetColumnLineColor(COLORREF columnLineColor) { m_columnLineColor = columnLineColor; }
	/************************************************************************/

	/************************************************************************/
	// config for row lines.

	BOOL IsShowRowLine() const { return m_bShowRowLine; }
	void IsShowRowLine( BOOL bShowRowLine ) { m_bShowRowLine = bShowRowLine; }

	double GetRowLineThickness() const { return m_dRowLineThickness; }
	void SetRowLineThickness(double dRowLineThickness) { m_dRowLineThickness = dRowLineThickness; }

	LINE_STYLE_TYPE GetRowLineStyle() const { return m_rowLineStyle; }
	void SetRowLineStyle(LINE_STYLE_TYPE rowLineStyle) { m_rowLineStyle = rowLineStyle; }

	COLORREF GetRowLineColor() const { return m_rowLineColor; }
	void SetRowLineColor(COLORREF rowLineColor) { m_rowLineColor = rowLineColor; }
	/************************************************************************/

	void SetHeaderHeight(double dHeaderHeight) { m_dHeaderHeight = dHeaderHeight; }
	double GetHeaderHeight() { return m_dHeaderHeight; }

	void SetRowHeight(double dRowHeight) { m_dRowHeight = dRowHeight; }
	double GetRowHeight() { return m_dRowHeight; }

	void SetTableColumn(TableColumnListPtr pTableColumnList) { m_pTableColumnList = pTableColumnList; }
	TableColumnListPtr GetTableColumn() { return m_pTableColumnList; }

	void SetColumnData(ColumnDataListPtr pColumnDataList) { m_pColumnDataList = pColumnDataList; }
	ColumnDataListPtr GetColumnData() { return m_pColumnDataList; }

private:
	// config for column lines.
	BOOL m_bShowColumnLine;
	double m_dColumnLineThickness;
	LINE_STYLE_TYPE	m_columnLineStyle;
	COLORREF m_columnLineColor;

	// config for row lines.
	BOOL m_bShowRowLine;
	double m_dRowLineThickness;
	LINE_STYLE_TYPE	m_rowLineStyle;
	COLORREF m_rowLineColor;

	// the header height.
	double m_dHeaderHeight;

	// the row height.
	double m_dRowHeight;

	// the table column list.
	TableColumnListPtr m_pTableColumnList;

	// the data of all columns.
	ColumnDataListPtr m_pColumnDataList;
};

END_CUTLEADER_NAMESPACE()
