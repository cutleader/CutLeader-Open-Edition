#ifndef XMenu_H
#define XMenu_H

#include "clUILibCommon.h"
#include <afxtempl.h>
#include "XMenuData.h"

BEGIN_CUTLEADER_NAMESPACE()

// This class implements an owner drawn menu class that mimics the menu style used in XP, Office and Visual C++.

struct CMenuItemInfo : public 
// this fixes warning C4097: typedef-name 'MENUITEMINFO' used as 
// synonym for class-name 'tagMENUITEMINFOA'
#ifndef UNICODE  
								tagMENUITEMINFOA
#else
								tagMENUITEMINFOW
#endif
{
	CMenuItemInfo()
	{
		memset(this, 0, sizeof(MENUITEMINFO));
		cbSize = sizeof(MENUITEMINFO);
	}
};

// how the menu's are drawn, either original or XP style.
typedef enum 
{
	BCMENU_DRAWMODE_ORIGINAL,
	BCMENU_DRAWMODE_XP
} BC_MenuDrawMode;

// how seperators are handled when removing a menu.
typedef enum 
{
	BCMENU_NONE, 
	BCMENU_HEAD, 
	BCMENU_TAIL, 
	BCMENU_BOTH
} BC_Seperator;

// defines for unicode support
#ifndef UNICODE
#define AppendMenu AppendMenuA
#define InsertMenu InsertMenuA
#define InsertODMenu InsertODMenuA
#define AppendODMenu AppendODMenuA
#define AppendODPopupMenu AppendODPopupMenuA
#define ModifyODMenu ModifyODMenuA
#else
#define AppendMenu AppendMenuW
#define InsertMenu InsertMenuW
#define InsertODMenu InsertODMenuW
#define AppendODMenu AppendODMenuW
#define ModifyODMenu ModifyODMenuW
#define AppendODPopupMenu AppendODPopupMenuW
#endif

class ClUILib_Export XMenu : public CMenu
{
	DECLARE_DYNAMIC(XMenu)

public:
	XMenu();

	virtual ~XMenu();

public:
	// Functions for loading and applying bitmaps to menus (see example application)
	virtual BOOL LoadMenu(LPCTSTR lpszResourceName);
	virtual BOOL LoadMenu(int nResource);
	BOOL LoadToolbar(UINT nToolBar);
	BOOL LoadToolbars(const UINT *arID,int n);
	void AddFromToolBar(CToolBar* pToolBar, int nResourceID);
	BOOL LoadFromToolBar(UINT nID,UINT nToolBar,int& xoffset);
	BOOL AddBitmapToImageList(CImageList *list,UINT nResourceID);
	static HBITMAP LoadSysColorBitmap(int nResourceId);
	// custom check mark bitmaps
	void LoadCheckmarkBitmap(int unselect,int select);
	
	// functions for appending a menu option, use the AppendMenu call 
	// (see above define).
	BOOL AppendMenuA(UINT nFlags, UINT nIDNewItem = 0, const char* lpszNewItem = NULL,
					 int nIconNormal = -1);
	BOOL AppendMenuA(UINT nFlags, UINT nIDNewItem, const char* lpszNewItem,
					 CImageList* il, int xoffset);
	BOOL AppendMenuA(UINT nFlags, UINT nIDNewItem, const char* lpszNewItem, 
					 CBitmap* bmp);
	BOOL AppendMenuW(UINT nFlags, UINT nIDNewItem = 0, 
					 wchar_t* lpszNewItem = NULL, int nIconNormal = -1);
	BOOL AppendMenuW(UINT nFlags, UINT nIDNewItem, wchar_t* lpszNewItem, 
					 CImageList* il, int xoffset);
	BOOL AppendMenuW(UINT nFlags, UINT nIDNewItem, wchar_t* lpszNewItem, CBitmap* bmp);
	BOOL AppendODMenuA(LPCSTR lpstrText, UINT nFlags = MF_OWNERDRAW,
					   UINT nID = 0, int nIconNormal = -1);  
	BOOL AppendODMenuW(wchar_t* lpstrText, UINT nFlags = MF_OWNERDRAW,
					   UINT nID = 0, int nIconNormal = -1);  
	BOOL AppendODMenuA(LPCSTR lpstrText, UINT nFlags, UINT nID, CImageList* il,
					   int xoffset);
	BOOL AppendODMenuW(wchar_t* lpstrText, UINT nFlags, UINT nID, CImageList* il,
					   int xoffset);
	
	// for appending a popup menu (see example application)
	XMenu* AppendODPopupMenuA(LPCSTR lpstrText);
	XMenu* AppendODPopupMenuW(wchar_t* lpstrText);

	// functions for inserting a menu option, use the InsertMenu call 
	// (see above define)
	BOOL InsertMenuA(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0, 
					 const char* lpszNewItem = NULL, int nIconNormal = -1);
	BOOL InsertMenuA(UINT nPosition, UINT nFlags, UINT nIDNewItem,
					 const char* lpszNewItem, CImageList* il, int xoffset);
	BOOL InsertMenuA(UINT nPosition, UINT nFlags, UINT nIDNewItem,
					 const char* lpszNewItem, CBitmap* bmp);
	BOOL InsertMenuW(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0,
					 wchar_t* lpszNewItem = NULL, int nIconNormal = -1);
	BOOL InsertMenuW(UINT nPosition, UINT nFlags, UINT nIDNewItem,
					 wchar_t* lpszNewItem, CImageList* il, int xoffset);
	BOOL InsertMenuW(UINT nPosition, UINT nFlags, UINT nIDNewItem,
					 wchar_t* lpszNewItem, CBitmap* bmp);
	BOOL InsertODMenuA(UINT nPosition, LPCSTR lpstrText, UINT nFlags = MF_OWNERDRAW,
					   UINT nID = 0, int nIconNormal = -1); 
	BOOL InsertODMenuW(UINT nPosition, wchar_t* lpstrText, UINT nFlags = MF_OWNERDRAW,
					   UINT nID = 0, int nIconNormal = -1);  
	BOOL InsertODMenuA(UINT nPosition, LPCSTR lpstrText, UINT nFlags, UINT nID,
					   CImageList* il, int xoffset);
	BOOL InsertODMenuW(UINT nPosition, wchar_t* lpstrText, UINT nFlags, UINT nID,
					   CImageList* il, int xoffset);
	
	// functions for modifying a menu option, use the ModifyODMenu call 
	// (see above define)
	BOOL ModifyODMenuA(const char* lpstrText, UINT nID = 0, int nIconNormal = -1);
	BOOL ModifyODMenuA(const char* lpstrText, UINT nID, CImageList* il, int xoffset);
	BOOL ModifyODMenuA(const char* lpstrText, UINT nID, CBitmap* bmp);
	BOOL ModifyODMenuA(const char* lpstrText, const char* OptionText, int nIconNormal);
	BOOL ModifyODMenuW(wchar_t* lpstrText, UINT nID = 0, int nIconNormal = -1);
	BOOL ModifyODMenuW(wchar_t* lpstrText, UINT nID, CImageList* il, int xoffset);
	BOOL ModifyODMenuW(wchar_t* lpstrText, UINT nID, CBitmap* bmp);
	BOOL ModifyODMenuW(wchar_t* lpstrText, wchar_t* OptionText, int nIconNormal);
	// use this method for adding a solid/hatched colored square beside a menu option
	// courtesy of Warren Stevens
	BOOL ModifyODMenuA(const char* lpstrText, UINT nID, COLORREF fill,
					   COLORREF border, int hatchstyle = -1, CSize* pSize = NULL);
	BOOL ModifyODMenuW(wchar_t* lpstrText, UINT nID, COLORREF fill,
					   COLORREF border, int hatchstyle = -1, CSize *pSize = NULL);

	// for deleting and removing menu options
	BOOL RemoveMenu(UINT uiId, UINT nFlags);
	BOOL DeleteMenu(UINT uiId, UINT nFlags);
	// sPos means Seperator's position, since we have no way to find the seperator's position in the menu
	// we have to specify them when we call the RemoveMenu to make sure the unused seperators are removed;
	// sPos  = None no seperator removal;
	//       = Head  seperator in front of this menu item;
	//       = Tail  seperator right after this menu item;
	//       = Both  seperators at both ends;
	// remove the menu item based on their text, return -1 if not found, otherwise return the menu position;
	int RemoveMenu(char* pText, BC_Seperator sPos = BCMENU_NONE);
	int RemoveMenu(wchar_t* pText, BC_Seperator sPos = BCMENU_NONE);
	int DeleteMenu(char* pText, BC_Seperator sPos = BCMENU_NONE);
	int DeleteMenu(wchar_t* pText, BC_Seperator sPos = BCMENU_NONE);
	
	// Destoying
	virtual BOOL DestroyMenu();

	// function for retrieving and setting a menu options text (use this function
	// because it is ownerdrawn)
	BOOL GetMenuText(UINT id, CString &string, UINT nFlags = MF_BYPOSITION);
	BOOL SetMenuText(UINT id, CString string, UINT nFlags = MF_BYPOSITION);

	// Getting a submenu from it's name or position
	XMenu* GetSubBCMenu(char* lpszSubMenuName);
	XMenu* GetSubBCMenu(wchar_t* lpszSubMenuName);
	CMenu* GetSubMenu(LPCTSTR lpszSubMenuName);
	CMenu* GetSubMenu(int nPos);
	int GetMenuPosition(char* pText);
	int GetMenuPosition(wchar_t* pText);

	// Called by the framework when a particular item needs to be drawn.  We
	// override this to draw the menu item in a custom-fashion, including icons
	// and the 3D rectangle bar.
	virtual void DrawItem(LPDRAWITEMSTRUCT);

	// Called by the framework when it wants to know what the width and height
	// of our item will be.  To accomplish this we provide the width of the
	// icon plus the width of the menu text, and then the height of the icon.
	virtual void MeasureItem(LPMEASUREITEMSTRUCT);

	// Static functions used for handling menu's in the mainframe
	static void UpdateMenu(CMenu* pmenu);
	static BOOL IsMenu(CMenu* submenu);
	static BOOL IsMenu(HMENU submenu);
	static LRESULT FindKeyboardShortcut(UINT nChar,UINT nFlags,CMenu *pMenu);

	// Function to set how menu is drawn, either original or XP style
	static void SetMenuDrawMode(UINT mode)
	{
		XMenu::original_drawmode = mode;
		XMenu::xp_drawmode = mode;
	};
	// Function to set how disabled items are drawn (mode=FALSE means they are not drawn selected)
	static void SetSelectDisableMode(BOOL mode)
	{
		XMenu::original_select_disabled = mode;
		XMenu::xp_select_disabled = mode;
	};

	static int XMenu::GetMenuDrawMode(void);
	static BOOL XMenu::GetSelectDisableMode(void);

	// how the bitmaps are drawn in XP Luna mode
	static void SetXPBitmap3D(BOOL val)
	{
		XMenu::xp_draw_3D_bitmaps=val;
	};
	static BOOL GetXPBitmap3D(void)
	{
		return XMenu::xp_draw_3D_bitmaps;
	}

	// Customizing:
	// Set icon size
	static void SetIconSize (int, int); 
	// set the color in the bitmaps that is the background transparent color
	void SetBitmapBackground(COLORREF color);
	void UnSetBitmapBackground(void);
	// obsolete functions for setting how menu images are dithered for disabled menu options
	BOOL GetDisableOldStyle(void);
	void SetDisableOldStyle(void);
	void UnSetDisableOldStyle(void);
	static COLORREF LightenColor(COLORREF col, double factor);
	static COLORREF DarkenColor(COLORREF col, double factor);

// Miscellaneous Protected Member functions
protected:
	static BOOL IsNewShell(void);
	static BOOL IsWinXPLuna(void);
	static BOOL IsLunaMenuStyle(void);
	static BOOL IsWindowsClassicTheme(void);
	XMenuData *XMenu::FindMenuItem(UINT nID);
	XMenu *FindMenuOption(int nId, int& nLoc);
	XMenu *FindAnotherMenuOption(int nId,int& nLoc, CArray<XMenu*,XMenu*>&bcsubs,
								  CArray<int,int&>& bclocs);
	XMenuData *FindMenuOption(wchar_t* lpstrText);
	void InsertSpaces(void);
	void DrawCheckMark(CDC* pDC, int x, int y, COLORREF color, BOOL narrowflag = FALSE);
	void DrawRadioDot(CDC *pDC, int x, int y, COLORREF color);
	XMenuData* NewODMenu(UINT pos, UINT nFlags, UINT nID, CString string);
	void SynchronizeMenu(void);
	void XMenu::InitializeMenuList(int value);
	void XMenu::DeleteMenuList(void);
	XMenuData* XMenu::FindMenuList(UINT nID);
	void DrawItem_Win9xNT2000(LPDRAWITEMSTRUCT lpDIS);
	void DrawItem_WinXP (LPDRAWITEMSTRUCT lpDIS);
	BOOL Draw3DCheckmark(CDC *dc, const CRect& rc, BOOL bSelected, HBITMAP hbmCheck);
	BOOL DrawXPCheckmark(CDC *dc, const CRect& rc, HBITMAP hbmCheck, 
						 COLORREF &colorout, BOOL selected);
	void DitherBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, 
				   int nHeight, HBITMAP hbm, int nXSrc, int nYSrc,COLORREF bgcolor);
	void DitherBlt2(CDC *drawdc, int nXDest, int nYDest, int nWidth, 
					int nHeight, CBitmap &bmp, int nXSrc, int nYSrc,COLORREF bgcolor);
	void DitherBlt3(CDC *drawdc, int nXDest, int nYDest, int nWidth, 
					int nHeight, CBitmap &bmp, COLORREF bgcolor);
	BOOL GetBitmapFromImageList(CDC* pDC, CImageList *imglist, int nIndex, 
								CBitmap &bmp);
	BOOL ImageListDuplicate(CImageList *il, int xoffset, CImageList *newlist);
	static WORD NumBitmapColors(LPBITMAPINFOHEADER lpBitmap);
	void ColorBitmap(CDC* pDC, CBitmap& bmp, CSize bitmap_size, CSize icon_size,
					 COLORREF fill, COLORREF border, int hatchstyle = -1);
	void RemoveTopLevelOwnerDraw(void);
	int GetMenuStart(void);
	void GetFadedBitmap(CBitmap &bmp);
	void GetTransparentBitmap(CBitmap &bmp);
	void GetDisabledBitmap(CBitmap &bmp,COLORREF background = 0);
	void GetShadowBitmap(CBitmap &bmp);
	int AddToGlobalImageList(CImageList *il,int xoffset, int nID);
	int GlobalImageListOffset(int nID);
	BOOL CanDraw3DImageList(int offset);
	
// Member Variables
protected:
	// Stores list of menu items
	CTypedPtrArray<CPtrArray, XMenuData*> m_MenuList;

	// Stores list of sub-menus.
	// notes:
	//   1) When loading an owner-drawn menu using a Resource, XMenu must 
	//      keep track of the popup menu's that it creates. Warning, 
	//      this list *MUST* be destroyed last item first.
	CTypedPtrArray<CPtrArray, HMENU>  m_SubMenus;

	// Stores a list of all XMenu's ever created 
	static CTypedPtrArray<CPtrArray, HMENU> m_AllSubMenus;
	
	// Global ImageList
	static CImageList m_AllImages;
	static CArray<int,int&> m_AllImagesID;
	
	// icon size
	static int m_iconX;
	static int m_iconY;

	COLORREF m_bitmapBackground;
	BOOL m_bitmapBackgroundFlag;
	BOOL disable_old_style;
	CImageList *checkmaps;
	BOOL checkmapsshare;
	int m_selectcheck;
	int m_unselectcheck;
	BOOL m_bDynIcons;
	BOOL m_loadmenu;

protected:
	static UINT original_drawmode;
	static BOOL original_select_disabled;
	static UINT xp_drawmode;
	static BOOL xp_select_disabled;
	static BOOL xp_draw_3D_bitmaps;
	static BOOL hicolor_bitmaps;
	static BOOL xp_space_accelerators;
	static BOOL original_space_accelerators;
}; 

END_CUTLEADER_NAMESPACE()

#endif
