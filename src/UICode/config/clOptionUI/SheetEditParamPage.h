#pragma once

#include "clOptionUICommon.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "NumEdit.h"

DECLARE_CUTLEADER_CLASS(SheetEditParam)

BEGIN_CUTLEADER_NAMESPACE()

class SheetEditParamPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(SheetEditParamPage)

public:
	SheetEditParamPage();
	~SheetEditParamPage();

	enum { IDD = IDD_clOptionUI_SheetEdit };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	DECLARE_MESSAGE_MAP()

public:
	void DisplaySheetEditParam(SheetEditParamPtr pSheetEditParam);
	SheetEditParamPtr GetSheetEditParam();

private: // controls
	// 步进值。
	CNumEdit m_editMoveStep;
	CNumEdit m_editRotStep;

	// 零件位置检测。
	BOOL m_bDetectPartDis;
	BOOL m_bDetectAlignment;
	BOOL m_bDetectCommon;
	CNumEdit m_editSheetMargin;
	CNumEdit m_editPartDis;
	CNumEdit m_editStickDis;
	CNumEdit m_editEscapeDis;

	// 光顺控制。
	BOOL m_bSleekCtrl;
	CNumEdit m_editSleekRate;

	// 工序。
	CNumEdit m_editLoopDetectDis;

private:
	SheetEditParamPtr m_pSheetEditParam;
};

END_CUTLEADER_NAMESPACE()
