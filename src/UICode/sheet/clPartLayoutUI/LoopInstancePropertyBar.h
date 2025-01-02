#pragma once

#include "PartLayoutPropertyBar.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(LoopInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

class ClPartLayoutUI_Export LoopInstancePropertyBar : public PartLayoutPropertyBar
{
private:
	LoopInstancePropertyBar(void);

public:
	virtual ~LoopInstancePropertyBar(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

public:
	static LoopInstancePropertyBar* GetInstance(CWnd* pParent = NULL);

	void DisplayProperty(const LoopInstanceList* pLoopInstances);

private:
	void AdjustLayout();
	BOOL CreatePropWnd();

private: // for UI.
	// the singleton object
	static LoopInstancePropertyBar* m_pLoopInstancePropertyBar;

	CMFCPropertyGridCtrl m_wndPropList;

	// use this brush to paint the background.
	CBrush m_brush;
};

END_CUTLEADER_NAMESPACE()
