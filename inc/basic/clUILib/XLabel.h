#pragma once

#include "ClUILibCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

#define	NM_LINKCLICK	WM_APP + 0x200

// 强大的静态文本控件
class ClUILib_Export XLabel : public CStatic
{
public:
	enum LinkStyle { LinkNone, HyperLink, MailLink };
	enum FlashType {None, Text, Background };
	enum Type3D { Raised, Sunken};
	enum BackFillMode { Normal, Gradient };

	XLabel();
	virtual ~XLabel();

public:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual XLabel& SetBkColor(COLORREF crBkgnd, COLORREF crBkgndHigh = 0, BackFillMode mode = Normal);
	virtual XLabel& SetTextColor(COLORREF crText);
	virtual XLabel& SetText(const CString& strText);
	virtual XLabel& SetFontBold(BOOL bBold);
	virtual XLabel& SetFontName(const CString& strFont, BYTE byCharSet = ANSI_CHARSET);
	virtual XLabel& SetFontUnderline(BOOL bSet);
	virtual XLabel& SetFontItalic(BOOL bSet);
	virtual XLabel& SetFontSize(int nSize);
	virtual XLabel& SetSunken(BOOL bSet);
	virtual XLabel& SetBorder(BOOL bSet);
	virtual XLabel& SetTransparent(BOOL bSet);
	virtual XLabel& FlashText(BOOL bActivate);
	virtual XLabel& FlashBackground(BOOL bActivate);
	virtual XLabel& SetLink(BOOL bLink,BOOL bNotifyParent);
	virtual XLabel& SetLinkCursor(HCURSOR hCursor);
	virtual XLabel& SetFont3D(BOOL bSet,Type3D type=Raised);
	virtual XLabel& SetRotationAngle(UINT nAngle,BOOL bRotation);
	virtual XLabel& SetText3DHiliteColor(COLORREF cr3DHiliteColor);
	virtual XLabel& SetFont(LOGFONT lf);
	virtual XLabel& SetMailLink(BOOL bEnable, BOOL bNotifyParent);
	virtual XLabel& SetHyperLink(const CString& sLink);

protected:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSysColorChange();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

protected:
	void UpdateSurface();
	void ReconstructFont();
	void DrawGradientFill(CDC* pDC, CRect* pRect, COLORREF crStart, COLORREF crEnd, int nSegments);

protected:
	COLORREF		m_crText;
	COLORREF		m_cr3DHiliteColor;
	HBRUSH			m_hwndBrush;
	HBRUSH			m_hBackBrush;
	LOGFONT			m_lf;
	CFont			m_font;
	BOOL			m_bState;
	BOOL			m_bTimer;
	LinkStyle		m_Link;
	BOOL			m_bTransparent;
	BOOL			m_bFont3d;
	BOOL			m_bToolTips;
	BOOL			m_bNotifyParent;
	BOOL			m_bRotation;
	FlashType		m_Type;
	HCURSOR			m_hCursor;
	Type3D			m_3dType;
	BackFillMode	m_fillmode;
	COLORREF		m_crHiColor;
	COLORREF		m_crLoColor;
	CString			m_sLink;
};

END_CUTLEADER_NAMESPACE()
