#pragma once

#include "clUILibCommon.h"
#include "CGridListCtrlGroups.h"
#include "XTreeCtrl.h"
#include "InvCellGrid.h"
#include "baseConst.h"
#include "CLDialog.h"
#include "afxvisualmanageroffice2007.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClUILib_Export SkinManager : public CMFCVisualManagerOffice2007
{
	DECLARE_DYNCREATE(SkinManager)

public:
	SkinManager(void);
	virtual ~SkinManager(void);

public:
	virtual void OnUpdateSystemColors();

	virtual BOOL GetToolTipInfo(CMFCToolTipInfo& params, UINT nType = (UINT)(-1));
	virtual void OnDrawRibbonMainPanelFrame(CDC* pDC, CMFCRibbonMainPanel* pPanel, CRect rect);
	virtual COLORREF OnDrawRibbonPanel(CDC *pDC, CMFCRibbonPanel *pPanel, CRect rectPanel, CRect rectCaption);
	virtual void OnDrawRibbonCategory(CDC* pDC, CMFCRibbonCategory* pCategory, CRect rectCategory);
	virtual void OnHighlightMenuItem(CDC *pDC, CMFCToolBarMenuButton* pButton, CRect rect, COLORREF& clrText);
	virtual COLORREF OnDrawMenuLabel(CDC* pDC, CRect rect);
	virtual void OnFillBarBackground(CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, BOOL bNCArea = FALSE);
	virtual COLORREF OnDrawPaneCaption(CDC* pDC, CDockablePane* pBar, BOOL bActive, CRect rectCaption, CRect rectButtons);
	virtual COLORREF OnFillRibbonButton(CDC* pDC, CMFCRibbonButton* pButton);
	virtual void OnFillRibbonMenuFrame(CDC* pDC, CMFCRibbonMainPanel* pPanel, CRect rect);
	virtual void OnFillRibbonQuickAccessToolBarPopup(CDC* pDC, CMFCRibbonPanelMenuBar* pMenuBar, CRect rect);
	virtual COLORREF OnDrawRibbonTabsFrame(CDC* pDC, CMFCRibbonBar* pWndRibbonBar, CRect rectTab);
	virtual void OnDrawSeparator(CDC* pDC, CBasePane* pBar, CRect rect, BOOL bIsHoriz);

	virtual void OnDrawTab(CDC* pDC, CRect rectTab, int iTab, BOOL bIsActive, const CMFCBaseTabCtrl* pTabWnd);
	virtual void OnDrawTabCloseButton(CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled);
	virtual COLORREF OnDrawPopupWindowCaption(CDC* pDC, CRect rectCaption, CMFCDesktopAlertWnd* pPopupWnd);

public:
	static CBrush GetDialogBkgndBrush();
	static HBRUSH GetDialogCtrlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor);
	static COLORREF GetTextColor();
	static COLORREF GetHighlightTextColor();
	static COLORREF GetBackgroundColor();
	static COLORREF GetDisableTextColor();
	static COLORREF GetSeparatorLineColor();
	static void SetTreeCtrlSelectedRowColor( NMHDR *pNMHDR, LRESULT *pResult );
	static void SetListCtrlSelectedRowColor( CListCtrl *ListCtrl, NMHDR *pNMHDR, LRESULT *pResult );

	// 在一个控件上绘制旋转的文字，最常用的是在按钮上绘制。
	// 注：
	// 1) 角度是整数值，不是弧度，以水平放置为参考进行旋转。
	static void DrawRotateText(CDC* pDC, const CString& strText, int iRotateAngle);

	static void Draw3DStyleFrame(CDC *pDC, const CRect reCtrl, const COLORREF crCtrlBkColor);
	static void OnBtnHighlight(CDC *pDC, const CRect rectButton);
	static void OnBtnFocus(CDC *pDC, const CRect rectButton);

private:
	void InitStyleColor();
};

class ClUILib_Export SkinBitmapButton : public CBitmapButton
{
	DECLARE_DYNAMIC( SkinBitmapButton )

public:
	SkinBitmapButton();
	virtual ~SkinBitmapButton();

protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

public:
	void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct ) override;

public:
	BOOL LoadBitmaps(UINT nIDBitmapResource, UINT nIDBitmapResourceSel = 0, UINT nIDBitmapResourceFocus = 0, UINT nIDBitmapResourceDisabled = 0);
};

class ClUILib_Export SkinMFCButton : public CMFCButton
{
	DECLARE_DYNAMIC(SkinMFCButton)

public:
	SkinMFCButton(void);
	virtual ~SkinMFCButton(void);

protected:
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;

public:
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	DECLARE_MESSAGE_MAP()

public:
	void SetImage(UINT uiBmpResId, UINT uiBmpHotResId = 0, UINT uiBmpDsblResID = 0);

private:
	void DrawText(CString szText);
	BOOL ImageFromIDResource(UINT nImageID);
	BOOL DrawImage();
	void PremultipliedAlpha();
	CPoint CalculateAlignPoint(bool bImage);

private:
	UINT m_nImageResID;
	UINT m_nImageHotResID;
	UINT m_nImageDsblResID;
	UINT m_nDisplayImageID;
	CImage m_image;
	CRect m_reCtrl;
	CDC *m_pDC;
	BOOL m_bDrawImage;
	CSize m_sizeText;
	CSize m_sizeImage;
};

class ClUILib_Export SkinMFCMenuButton : public CMFCMenuButton
{
	DECLARE_DYNAMIC( SkinMFCMenuButton )

public:
	SkinMFCMenuButton();
	virtual ~SkinMFCMenuButton();
};

class ClUILib_Export SkinListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC( SkinListCtrl )

public:
	SkinListCtrl(void);
	virtual ~SkinListCtrl(void);

protected:
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
	afx_msg void OnNMCustomdraw( NMHDR *pNMHDR, LRESULT *pResult );
	DECLARE_MESSAGE_MAP()
};

class ClUILib_Export SkinGridListCtrlGroups : public CGridListCtrlGroups
{
	DECLARE_DYNAMIC( SkinGridListCtrlGroups )

public:
	SkinGridListCtrlGroups(void);
	virtual ~SkinGridListCtrlGroups(void);

protected:
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
	afx_msg void OnNMCustomdraw( NMHDR *pNMHDR, LRESULT *pResult );
	DECLARE_MESSAGE_MAP()

public:
	bool OnDisplayRowColor(int nRow, COLORREF textColor, COLORREF backColor);
};

class ClUILib_Export SkinInvCellGrid : public InvCellGrid
{
	DECLARE_DYNAMIC( SkinInvCellGrid )

public:
	SkinInvCellGrid(void);
	virtual ~SkinInvCellGrid(void);

protected:
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
	afx_msg void OnNMCustomdraw( NMHDR *pNMHDR, LRESULT *pResult );
	DECLARE_MESSAGE_MAP()

public:
	bool OnDisplayRowColor(int nRow, COLORREF textColor, COLORREF backColor);
};

class ClUILib_Export SkinEdit : public CEdit
{
	DECLARE_DYNAMIC( SkinEdit )

public:
	SkinEdit(void);
	virtual ~SkinEdit(void);

public:
	virtual BOOL PreTranslateMessage(MSG *pMsg);

protected:
	afx_msg HBRUSH CtlColor( CDC *pDC, UINT nCtlColor );
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
};

class ClUILib_Export SkinStatic : public CStatic
{
	DECLARE_DYNAMIC( SkinStatic )

public:
	SkinStatic(void);
	virtual ~SkinStatic(void);

protected:
	afx_msg HBRUSH CtlColor ( CDC *pDC, UINT nCtlColor );
	DECLARE_MESSAGE_MAP()
};

class ClUILib_Export SkinTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC( SkinTreeCtrl )

public:
	SkinTreeCtrl(void);
	virtual ~SkinTreeCtrl(void);

protected:
	afx_msg void OnNMCustomdraw( NMHDR *pNMHDR, LRESULT *pResult );
	DECLARE_MESSAGE_MAP()

public:
	void Init();
};

class ClUILib_Export SkinXTreeCtrl : public XTreeCtrl
{
	DECLARE_DYNAMIC( SkinXTreeCtrl )

public:
	SkinXTreeCtrl();
	SkinXTreeCtrl(int iCopyDropCur, int iMoveDropCur, int iNoDropCur);
	virtual ~SkinXTreeCtrl();

protected:
	afx_msg void OnNMCustomdraw( NMHDR *pNMHDR, LRESULT *pResult );
	DECLARE_MESSAGE_MAP()

public:
	void Init();
};

class ClUILib_Export SkinTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(SkinTabCtrl )

public:
	SkinTabCtrl();
	virtual ~SkinTabCtrl();

protected:
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
	DECLARE_MESSAGE_MAP()

public:
	void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct ) override;

public:
	void SetTabColor( int nIndex, COLORREF color );

private:
	COLORREF m_tabColor[12];
};

END_CUTLEADER_NAMESPACE()
