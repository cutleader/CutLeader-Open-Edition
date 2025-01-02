#pragma once

#include "clUILibCommon.h"
#include <string>
#include <afxmt.h>
#include <list>

BEGIN_CUTLEADER_NAMESPACE()

class ClUILib_Export BalloonHelp : public CWnd
{
public:
	BalloonHelp();
	virtual ~BalloonHelp();

public:
	// options
	static const unsigned int unCLOSE_ON_LBUTTON_UP;   // closes window on WM_LBUTTON_UP
	static const unsigned int unCLOSE_ON_MBUTTON_UP;   // closes window on WM_MBUTTON_UP
	static const unsigned int unCLOSE_ON_RBUTTON_UP;   // closes window on WM_RBUTTON_UP
	static const unsigned int unCLOSE_ON_LBUTTON_DOWN; // closes window on WM_LBUTTON_DOWN
	static const unsigned int unCLOSE_ON_MBUTTON_DOWN; // closes window on WM_MBUTTON_DOWN
	static const unsigned int unCLOSE_ON_RBUTTON_DOWN; // closes window on WM_RBUTTON_DOWN
	static const unsigned int unCLOSE_ON_MOUSE_MOVE;   // closes window when user moves mouse past threshhold
	static const unsigned int unCLOSE_ON_KEYPRESS;     // closes window on the next keypress message sent to this thread.
	static const unsigned int unCLOSE_ON_ANYTHING;     // all of the above
	static const unsigned int unDELAY_CLOSE;           // when a user action triggers the close, begins timer.  closes when timer expires.
	static const unsigned int unDELETE_THIS_ON_CLOSE;  // deletes object when window is closed.  Used by LaunchBalloon(), use with care
	static const unsigned int unSHOW_CLOSE_BUTTON;     // shows close button in upper right
	static const unsigned int unSHOW_INNER_SHADOW;     // draw inner shadow in balloon
	static const unsigned int unSHOW_TOPMOST;          // place balloon above all other windows
	static const unsigned int unDISABLE_XP_SHADOW;     // disable Windows XP's drop-shadow effect (overrides system and user settings)
	static const unsigned int unDISABLE_FADEIN;        // disable the fade-in effect (overrides system and user settings)
	static const unsigned int unDISABLE_FADEOUT;       // disable the fade-out effect (overrides system and user settings)
	static const unsigned int unDISABLE_FADE;          // disable the fade-in/fade-out effects (overrides system and user settings)

	BOOL Create(const CString& strTitle,         // title of balloon
		const CString& strContent,       // content of balloon
		const CPoint& ptAnchor,          // anchor (tail position) of balloon
		unsigned int unOptions,          // options (see above)
		CWnd* pParentWnd = NULL,         // parent window (NULL == MFC main window)
		const CString strURL = _T(""),       // URL to open (ShellExecute()) when clicked
		unsigned int unTimeout = 0,      // delay before closing automatically (milliseconds)
		HICON hIcon = NULL);             // icon to display

	// Show a help balloon on screen.
	static void LaunchBalloon(const CString& strTitle, const CString& strContent, 
		const CPoint& ptAnchor, 
		LPCTSTR szIcon = IDI_EXCLAMATION,
		unsigned int unOptions = unSHOW_CLOSE_BUTTON,
		CWnd* pParentWnd = NULL, 
		const CString strURL = _T(""),
		unsigned int unTimeout = 10000);

	// Sets the font used for drawing the balloon title.  Deleted by balloon, do not use CFont* after passing to this function.
	void SetTitleFont(CFont* pFont);
	// Sets the font used for drawing the balloon content.  Deleted by balloon, do not use CFont* after passing to this function.
	void SetContentFont(CFont* pFont);
	// Sets the icon displayed in the top left of the balloon (pass NULL to hide icon)
	void SetIcon(HICON hIcon);
	// Sets the icon displayed in the top left of the balloon (pass NULL to hide icon)
	void SetIconScaled(HICON hIcon, int cx, int cy);
	// Sets the icon displayed in the top left of the balloon (pass NULL hBitmap to hide icon)
	void SetIcon(HBITMAP hBitmap, COLORREF crMask);
	// Sets the icon displayed in the top left of the balloon
	void SetIcon(HBITMAP hBitmap, HBITMAP hMask);
	// Set icon displayed in the top left of the balloon to image # nIconIndex from pImageList
	void SetIcon(CImageList* pImageList, int nIconIndex);
	// Sets the URL to be opened when balloon is clicked.  Pass "" to disable.
	void SetURL(const CString& strURL);
	// Sets the number of milliseconds the balloon can remain open.  Set to 0 to disable timeout.
	void SetTimeout(unsigned int unTimeout);
	// Sets the distance the mouse must move before the balloon closes when the unCLOSE_ON_MOUSE_MOVE option is set.
	void SetMouseMoveTolerance(int nTolerance);
	// Sets the point to which the balloon is "anchored"
	void SetAnchorPoint(CPoint ptAnchor, CWnd* pWndAnchor = NULL);
	// Sets the title of the balloon
	void SetTitle(const CString& strTitle);
	// Sets the content of the balloon (plain text only)
	void SetContent(const CString& strContent);
	// Sets the forground (text and border) color of the balloon
	void SetForegroundColor(COLORREF crForeground);
	// Sets the background color of the balloon
	void SetBackgroundColor(COLORREF crBackground);
protected:
	// layout constants
	static const int nTIP_TAIL;
	static const int nTIP_MARGIN;

	// calculate anchor position (adjust for client coordinates if used)
	CPoint GetAnchorPoint();

	// determine bounds of screen anchor is on (Multi-Monitor compatibility)
	void GetAnchorScreenBounds(CRect& rect);

	// determine section of the screen balloon is on
	enum BALLOON_QUADRANT { BQ_TOPRIGHT, BQ_TOPLEFT, BQ_BOTTOMRIGHT, BQ_BOTTOMLEFT };
	BALLOON_QUADRANT GetBalloonQuadrant();

	// Draw the non-client area
	virtual void DrawNonClientArea(CDC* pDC);
	// Draw the client area
	virtual void DrawClientArea(CDC* pDC);
	// Calculate the dimensions and draw the balloon header
	virtual CSize DrawHeader(CDC* pDC, bool bDraw = TRUE);
	// Calculate the dimensions and draw the balloon contents
	virtual CSize DrawContent(CDC* pDC, int nTop, bool bDraw = TRUE);
	// Calculate the dimensions required to draw the balloon header
	CSize CalcHeaderSize(CDC* pDC) { return DrawHeader(pDC, FALSE); }
	// Calculate the dimensions required to draw the balloon content
	CSize CalcContentSize(CDC* pDC) { return DrawContent(pDC, 0, FALSE); }
	// Calculate the total size needed by the balloon window
	CSize CalcWindowSize();
	// Calculate the total size needed by the client area of the balloon window
	CSize CalcClientSize();
	// Size and position the balloon window on the screen.
	void PositionWindow();

	// Displays the balloon on the screen, performing fade-in if enabled.
	void ShowBalloon(void);
	// Removes the balloon from the screen, performing the fade-out if enabled
	void HideBalloon(void);

	// Returns the class ATOM for a BalloonHelp control.  Registers the class first, if necessary.
	static ATOM GetClassAtom(BOOL bShadowed);

	// message handlers
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg LRESULT OnPrint(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnLButtonDown(UINT, CPoint point);
	afx_msg void OnLButtonUp(UINT, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
private:
	// Keyboard hook
	void SetKeyboardHook();
	void RemoveKeyboardHook();
	// Mouse hook
	void SetMouseHook();
	void RemoveMouseHook();
	// Call Window Return hook
	void SetCallWndRetHook();
	void RemoveCallWndRetHook();

protected:
	// Keyboard hook callback
	LRESULT KeyboardHookProc( int code, WPARAM wParam, LPARAM lParam);
	// Mouse hook callback
	LRESULT MouseHookProc(int code, WPARAM wParam, LPARAM lParam);
	// Call Window Return hook callback (automatic following)
	LRESULT CallWndRetProc(int code, WPARAM wParam, LPARAM lParam);

private:

	// animate window API, if available
	typedef BOOL (WINAPI* FN_ANIMATE_WINDOW)(HWND,DWORD,DWORD);
	FN_ANIMATE_WINDOW m_fnAnimateWindow;

	// hook handles, if set
	bool           m_bKeyboardHook;
	bool           m_bMouseHook;
	bool           m_bCallWndRetHook;

	unsigned int   m_unOptions;
	unsigned int   m_unTimeout;      // max time to show, in milliseconds
	UINT_PTR       m_unTimerClose;   // ID of kill timer
	CString        m_strContent;     // text to show in content area
	CString        m_strURL;         // url to open, if clicked.
	HWND           m_hwndAnchor;     // window to anchor to (can be NULL for desktop anchor)
	CPoint         m_ptAnchor;       // "anchor" (point of tail)
	CImageList     m_ilIcon;         // icon

	CFont*         m_pTitleFont;     // font to use for title
	CFont*         m_pContentFont;   // font to use for content

	COLORREF       m_crBackground;   // Background color for balloon   
	COLORREF       m_crForeground;   // Foreground color for balloon

	CRect          m_screenRect;     // bounds of screen anchor is on
	CRgn           m_rgnComplete;    // Clipping / Drawing region
	CPoint         m_ptMouseOrig;    // original mouse position; for hiding on mouse move
	UINT           m_uCloseState;    // current state of the close button
	int            m_nMouseMoveToleranceX;  // distance mouse has to move before balloon will close.
	int            m_nMouseMoveToleranceY;  // distance mouse has to move before balloon will close.

	// class atoms (shadowed/not shadowed)
	static ATOM    s_ClassAtom;
	static ATOM    s_ClassAtomShadowed;

protected:
	//Singleton that broadcast hook events to all classes that currently exist.
	class HookMan
	{
	public:
		// hook handles, if set
		HHOOK          m_hKeyboardHook;
		HHOOK          m_hMouseHook;
		HHOOK          m_hCallWndRetHook;
		std::list<BalloonHelp*> m_lstClasses;
		int m_nCountKeyboard;
		int m_nCountMouse;
		int m_nCountCallWnd;
		CCriticalSection m_cs;

		HookMan();
		~HookMan();
		void AddClass(BalloonHelp* pClass);
		void RemoveClass(BalloonHelp* pClass);

		// Keyboard hook
		void SetKeyboardHook();
		void RemoveKeyboardHook();
		// Mouse hook
		void SetMouseHook();
		void RemoveMouseHook();
		// Call Window Return hook
		void SetCallWndRetHook();
		void RemoveCallWndRetHook();

		static LRESULT CALLBACK KeyboardHookProcStatic( int code, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK MouseHookProcStatic(int code, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK CallWndRetProcStatic(int code, WPARAM wParam, LPARAM lParam);
	};
	friend class HookMan;
	static HookMan s_hookMan;
};

END_CUTLEADER_NAMESPACE()
