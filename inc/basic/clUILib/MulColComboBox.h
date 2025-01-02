#pragma once

#pragma warning ( disable : 4786 )

#include "clUILibCommon.h"
#include <map>
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

// this class implement Combo-box with a colored items, icons, bold items, and multiple columns.
// notes:
//   1) you should set the property of the combo box.
//      a) "Has string" as TRUE
//      b) "Owner Draw" as "Fixed".
class ClUILib_Export MulColComboBox : public CComboBox
{
public:
	MulColComboBox();
	virtual ~MulColComboBox();

public:
	struct ItemData
	{
		ItemData() : crTextColor(RGB(0,0,0)),nImageIndex(-1),bBold(FALSE){}
		COLORREF crTextColor;
		int nImageIndex;
		std::map<int,CString> mapStrings;
		BOOL bBold;
		DWORD dwItemData;
	};

	std::vector<int> m_vecColumnWidth;

	CImageList* m_pImageList;
	BOOL m_bUseImage;

	static const int DEFAULT_COLUMN_COUNT;
	static const int DEFAULT_COLUMN_WIDTH;

// Implementation
public:
	// Sets to TRUE for using the image list, of FALSE to disable 
	// the use of the image-list.
	void SetUseImage(BOOL bUseImage=TRUE) { m_bUseImage=bUseImage; Invalidate(); }

	// Sets the number of columns to use - new columns are inserted at 
	// default width.
	void SetColumnCount(int nColumnCount);

	// Sets the width of a specific column.
	void SetColumnWidth(int nColumnIndex, int nWidth);

	// Sets the image list to use - will be show only if SetUseImage is called.
	void SetImageList(CImageList* pImageList) { m_pImageList = pImageList; }

	// Set a specific row to bold.
	void SetItemBold(int nItemIndex, BOOL bBold = TRUE);

	// Sets a row's image index.
	void SetItemImage(int nItemIndex, int nImageIndex);

	// Sets a row's color.
	void SetItemColor(int nItemIndex, COLORREF rcTextColor);

	// Sets an item or sub-item text.
	void SetItemText(int nItemIndex, int nColumn, CString str);
	
	// Sets item data (override the default function)
	void SetItemData(int nItemIndex, DWORD dwData);

	// Get item data(override the default function)
	DWORD GetItemData(int nItemIndex);

	// Gets item or sub-item text.
	CString GetItemText(int nItemIndex,int nColumn);

protected:
	ItemData* GetOrCreateItemData(int nItemIndex);

	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	void MeasureItem(LPMEASUREITEMSTRUCT) {}

	afx_msg void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);

	DECLARE_MESSAGE_MAP()
};

END_CUTLEADER_NAMESPACE()
