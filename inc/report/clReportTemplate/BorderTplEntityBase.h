#pragma once

#include "TplEntityBase.h"
#include "IBorderTplEntity.h"

DECLARE_CUTLEADER_CLASS(IBorderTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

// the basic class for the entities which can display the border.
class BorderTplEntityBase : virtual public TplEntityBase,
							virtual public IBorderTplEntity
{
public:
	BorderTplEntityBase(void);
	~BorderTplEntityBase(void);

public: // implement interface IBorderTplEntity.
	virtual BOOL IsShowBorder() const { return m_bShowBorder; }
	virtual void IsShowBorder(BOOL bShowBorder) { m_bShowBorder = bShowBorder; }

	virtual int GetBorderThickness() const { return m_iBorderThickness; }
	virtual void SetBorderThickness(int iBorderThickness) { m_iBorderThickness = iBorderThickness; }

	virtual LINE_STYLE_TYPE GetBorderStyle() const { return m_borderLineStyle; }
	virtual void SetBorderStyle(LINE_STYLE_TYPE lineStyle) { m_borderLineStyle = lineStyle; }

	virtual COLORREF GetBorderColor() const { return m_borderColor; }
	virtual void SetBorderColor(COLORREF borderColor) { m_borderColor = borderColor; }

public:
	void UpdateBorderStyle(IBorderTplEntityPtr pBorderTplEntity) const;

private:
	// whether show the border.
	BOOL m_bShowBorder;

	// the border thickness.
	int m_iBorderThickness;

	// the border line style.
	LINE_STYLE_TYPE m_borderLineStyle;

	// the border color.
	COLORREF m_borderColor;
};

END_CUTLEADER_NAMESPACE()
