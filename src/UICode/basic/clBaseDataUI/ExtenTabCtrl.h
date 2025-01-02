#pragma once


// CExtenTabCtrl

class CExtenTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CExtenTabCtrl)

public:
	CExtenTabCtrl();
	virtual ~CExtenTabCtrl();

protected:
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
	DECLARE_MESSAGE_MAP()
public:
	void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct ) override;
	void SetTabColor( int nIndex, COLORREF color );
private:
	COLORREF m_tabColor[12];
};


