#pragma once

#include "clCutSequenceUICommon.h"
#include "taoresource.h"
#include "NumEdit.h"
#include "SubjectBase.h"
#include "Rect2D.h"
#include "SkinManager.h"
#include "ActionBar.h"

BEGIN_CUTLEADER_NAMESPACE()

// 发给action的命令
enum GenerateRemCutLineActionBar_Cmd
{
	GenerateRemCutLineActionBar_noCmd			= -1,
	GenerateRemCutLineActionBar_previewCmd	= 0,
	GenerateRemCutLineActionBar_generateCmd	= 1,
	GenerateRemCutLineActionBar_saveMatCmd	= 2,
};

class ClCutSequenceUI_Export GenerateRemCutLineActionBar : public ActionBar, public SubjectBase_2
{
	DECLARE_DYNAMIC(GenerateRemCutLineActionBar)

public:
	GenerateRemCutLineActionBar(CWnd* pParent = NULL);
	~GenerateRemCutLineActionBar(void);

	enum {
		IDD = IDD_clCutSequenceUI_CreateRemnantCutLineActionBar
	};

protected: // inherit from the base class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

public: // message mapping.
	afx_msg void OnPreview();
	afx_msg void OnSaveRemMat();
	afx_msg void OnGenerate();

	DECLARE_MESSAGE_MAP()

public:
	void Init(RECT_SIDE iRemnantPosition, double dStepWidth);
	RECT_SIDE GetRemnantPosition() const;
	RECT_SIDE GetRayDir() const;

	double GetOffsetDistance() { return m_ctrlOffsetDistance.Get_Number(); }
	int GetStepWidth() { return (int)m_ctrlStepWidth.Get_Number(); }

	GenerateRemCutLineActionBar_Cmd GetCmdType() const { return m_iCmdType; }

	void Show_GenerateRemCutLineActionBar();

private: // for controls.
	CComboBox m_comboRemnantPosition;
	CNumEdit m_ctrlOffsetDistance;
	CNumEdit m_ctrlStepWidth;

	SkinMFCButton m_btnPreciew;//IDC_BTN_PREVIEW
	SkinMFCButton m_btnOk;//IDC_BTN_OK
	SkinMFCButton m_btnSaveRemMat;//IDC_BTN_SAVE_REM_MAT

private: // data.
	GenerateRemCutLineActionBar_Cmd m_iCmdType;
};

END_CUTLEADER_NAMESPACE()
