#pragma once

#include "BorderTplEntityBase.h"
#include "BindableEntityBase.h"

DECLARE_CUTLEADER_CLASS(GridColumnList)
DECLARE_CUTLEADER_CLASS(GridTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

// the table entity.
class ClReportTemplate_Export GridTplEntity : virtual public BorderTplEntityBase,
										 virtual public BindableEntityBase
{
public:
	GridTplEntity(void);
	~GridTplEntity(void);

public: // functions from IData interface.
	virtual BOOL IsModified() const override;
	virtual IDataPtr Clone() const override;

public: // implement ITplEntity interface.
	virtual ENTITY_TYPE GetType() const { return ENTITY_GRID; }

public: // get/set functions.
	/************************************************************************/
	// config for column lines.

	BOOL IsShowColumnLine() const { return m_bShowColumnLine; }
	void IsShowColumnLine( BOOL bShowColumnLine ) { m_bShowColumnLine = bShowColumnLine; }

	int GetColumnLineThickness() const { return m_iColumnLineThickness; }
	void SetColumnLineThickness( int iColumnLineThickness ) { m_iColumnLineThickness = iColumnLineThickness; }

	LINE_STYLE_TYPE GetColumnLineStyle() const { return m_columnLineStyle; }
	void SetColumnLineStyle( LINE_STYLE_TYPE columnLineStyle ) { m_columnLineStyle = columnLineStyle; }

	COLORREF GetColumnLineColor() const { return m_columnLineColor; }
	void SetColumnLineColor( COLORREF columnLineColor ) { m_columnLineColor = columnLineColor; }
	/************************************************************************/

	/************************************************************************/
	// config for row lines.

	BOOL IsShowRowLine() const { return m_bShowRowLine; }
	void IsShowRowLine( BOOL bShowRowLine ) { m_bShowRowLine = bShowRowLine; }

	int GetRowLineThickness() const { return m_iRowLineThickness; }
	void SetRowLineThickness( int iRowLineThickness ) { m_iRowLineThickness = iRowLineThickness; }

	LINE_STYLE_TYPE GetRowLineStyle() const { return m_rowLineStyle; }
	void SetRowLineStyle( LINE_STYLE_TYPE rowLineStyle ) { m_rowLineStyle = rowLineStyle; }

	COLORREF GetRowLineColor() const { return m_rowLineColor; }
	void SetRowLineColor( COLORREF rowLineColor ) { m_rowLineColor = rowLineColor; }
	/************************************************************************/

	GridColumnListPtr GetGridColumnList() { return m_pGridColumnList; }
	void SetGridColumnList(GridColumnListPtr pGridColumnList) { m_pGridColumnList = pGridColumnList; }

	void SetRowHeight(int iRowHeight) { m_iRowHeight = iRowHeight; }
	int GetRowHeight() { return m_iRowHeight; }

	void SetHeaderHeight(int iHeaderHeight) { m_iHeaderHeight = iHeaderHeight; }
	int GetHeaderHeight() { return m_iHeaderHeight; }

public:
	// get the total width of the columns.
	int GetTotalColWidth();

	// get the total row count.
	// notes:
	//   1) do not include the header.
	int GetTotalRowCount();

	// get the useful rect of the grid entity.
	CRect GetUsefulRect();

private:
	// config for column lines.
	// notes:
	//   1) use intData5~intData8.
	BOOL m_bShowColumnLine;
	int	m_iColumnLineThickness;
	LINE_STYLE_TYPE	m_columnLineStyle;
	COLORREF m_columnLineColor;

	// config for row lines.
	// notes:
	//   1) use intData9~intData12.
	BOOL m_bShowRowLine;
	int m_iRowLineThickness;
	LINE_STYLE_TYPE	m_rowLineStyle;
	COLORREF m_rowLineColor;

	// each column definition.
	GridColumnListPtr m_pGridColumnList;

	// the row height.
	// notes:
	//   1) use intData13.
	int m_iRowHeight;

	// the header height.
	// notes:
	//   1) use intData14.
	int m_iHeaderHeight;
};

END_CUTLEADER_NAMESPACE()
