#pragma once

#include "clUILibCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// we can draw Images in the ImgStatic.
class ClUILib_Export ImgStatic : public CStatic
{
	DECLARE_DYNAMIC(ImgStatic)

public:
	ImgStatic(void);
	~ImgStatic(void);

public:
	void SetPicImageList(CImageList* pImageList, int iImageCount);

	int GetCurrentImage();
	void SetCurrentImage(int iImage);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
	// the image list
	CImageList *m_pImageList;

	// image count in "m_pImageList"
	int m_iImageCount;

	// current image index
	int m_iCurrentImage;
};

END_CUTLEADER_NAMESPACE()
