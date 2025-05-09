#pragma once

#include "clUILibCommon.h"
#include "XNamedColors.h"

BEGIN_CUTLEADER_NAMESPACE()

struct XHTMLSTATIC_APP_COMMAND
{
	HWND	hWnd;			// HWND of window to receive message
	UINT	uMessage;		// message sent to parent
	WPARAM	wParam;			// user defined data returned in wParam parameter
	TCHAR *	pszCommand;		// string that ties this entry to HTML hyperlink
};

struct XHTMLSTATIC_CHAR_ENTITIES
{
	TCHAR *	pszName;		// string entered in HTML - e.g., "&nbsp;"
	TCHAR	cCode;			// code generated by XHTMLStatic
	TCHAR	cSymbol;		// character symbol displayed
};

// the HTML style static control.
class ClUILib_Export XHTMLStatic : public CStatic
{
public:
	XHTMLStatic();
	virtual ~XHTMLStatic();

	void ResetAll();			// formerly called Init()
	void Reset();

// Attributes
public:
	virtual COLORREF SetBkColor(COLORREF rgb) 
	{
		COLORREF oldrgb = m_crBackGround;
		m_crBackGround = rgb;
		return oldrgb;
	}
	virtual COLORREF SetBkColor(LPCTSTR lpszColor) 
	{
		ASSERT(lpszColor);
		CXNamedColors nc(lpszColor);
		COLORREF oldrgb = m_crBackGround;
		m_crBackGround = nc.GetRGB();
		return oldrgb;
	}
	COLORREF GetBkColor() const { return m_crBackGround; }

	virtual COLORREF SetTextColor(COLORREF rgb) 
	{
		COLORREF oldrgb = m_crText;
		m_crText = rgb;
		return oldrgb;
	}
	virtual COLORREF SetTextColor(LPCTSTR lpszColor) 
	{
		ASSERT(lpszColor);
		CXNamedColors nc(lpszColor);
		COLORREF oldrgb = m_crText;
		m_crText = nc.GetRGB();
		return oldrgb;
	}
	COLORREF GetTextColor() const { return m_crText; }
	void SetMargins(int nLeftMargin, int nRightMargin)
	{
		m_nLeftMargin = nLeftMargin;
		m_nRightMargin = nRightMargin;
	}
	void SetAppCommands(XHTMLSTATIC_APP_COMMAND * paAppCommands, int nAppCommands)
	{
		if (m_paAppCommands)
			delete [] m_paAppCommands;
		m_paAppCommands = NULL;

		m_nAppCommands = 0;

		if (paAppCommands && (nAppCommands > 0))
		{
			m_paAppCommands = new XHTMLSTATIC_APP_COMMAND[nAppCommands];
			ASSERT(m_paAppCommands);
			memcpy(m_paAppCommands, paAppCommands, 
				sizeof(XHTMLSTATIC_APP_COMMAND) * nAppCommands);
			m_nAppCommands = nAppCommands;
		}
	}
	void SetLogFont(const LOGFONT * pLogFont)
	{
		ASSERT(pLogFont);
		memcpy(&m_lf, pLogFont, sizeof(m_lf));
		m_bLogFont = TRUE;
	}
	void SetWindowText(LPCTSTR lpszString);
	void EnableTooltip(BOOL bFlag) 
	{ 
		m_bToolTip = bFlag; 
	}
	BOOL IsTooltipEmabled() const 
	{ 
		return m_bToolTip; 
	}


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(XHTMLStatic)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	Draw(HDC hdc);
	int		FormatText(HDC hdc, LPCTSTR lpszText, RECT * pRect, int nInitialXOffset);
	TCHAR	GetCharEntity(TCHAR cCode);
	CString	GetNextWord(TCHAR **ppText, BOOL *pbReturnSeen);
	LONG	GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
	BOOL	GotoURL(LPCTSTR url, int showcmd);
	void	InitCharEntities();
	BOOL	IsBlank(LPCTSTR lpszText);
	BOOL	ProcessAppCommand(LPCTSTR lpszCommand);

	HDC				m_hMemDC;
	HBITMAP			m_hBitmap;
	HBITMAP			m_hOldBitmap;
    CToolTipCtrl	m_ToolTip;
	COLORREF		m_crBackGround;
	COLORREF		m_crText;
	LOGFONT			m_lf;
	BOOL			m_bRefresh;
	BOOL			m_bToolTip;
	BOOL			m_bLogFont;
	BOOL			m_bUnderline;
	BOOL			m_bBold;
	BOOL			m_bItalic;
	BOOL			m_bCenter;
	BOOL			m_bStrikeThrough;
	BOOL			m_bSubscript;
	BOOL			m_bSuperscript;
	BOOL			m_bHorizontalRule;
	BOOL			m_bGeneratedText;
	BOOL			m_bInAnchor;
	BOOL			m_bOnHyperlink;
	int				m_nHorizontalRuleSize;
	CPtrArray		m_AnchorRectPtrs;
	CStringArray	m_AnchorUrls;
	HCURSOR			m_hLinkCursor;
	HCURSOR			m_hPrevCursor;
	int				m_nLeftMargin, m_nRightMargin;
	int				m_yStart;
	int				m_nAppCommands;
	static XHTMLSTATIC_CHAR_ENTITIES	m_aCharEntities[];
	XHTMLSTATIC_APP_COMMAND *			m_paAppCommands;

	// Generated message map functions
protected:
	//{{AFX_MSG(XHTMLStatic)
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnClicked();
	LRESULT OnPrint(WPARAM wParam, LPARAM /*lParam*/);

	DECLARE_MESSAGE_MAP()
};

END_CUTLEADER_NAMESPACE()
