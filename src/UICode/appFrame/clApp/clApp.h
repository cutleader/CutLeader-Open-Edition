#pragma once

#include "resource.h"
#include "cutLeaderCommon.h"

DECLARE_CUTLEADER_CLASS(SplashThread)

class ClApp : public CWinAppEx
{
public:
	ClApp();

protected: // inherit from the super class.
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public: // message mapping.
	afx_msg void OnDataMgr();
	afx_msg void OnNewEmptyPart();
	afx_msg void OnNewPartFromDxfdwg();
	afx_msg void OnNewTaskFromEmptySheet();
	afx_msg void OnNewTaskFromNesting();

	afx_msg BOOL OnOpenRecentData(UINT nID);
	afx_msg void OnRecentData();

	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();

	// Configuration Category.
	afx_msg void OnCLOption();
	afx_msg void OnMacLib();
	afx_msg void OnMatLib();
	afx_msg void OnExpertLib();
	afx_msg void OnDflReportTpl();
	afx_msg void OnDesignReportTpl();
	afx_msg void OnSetDefaultParam();

	// "Help" Category.
	afx_msg void OnAbout();

	afx_msg void OnNewTaskAndAddDxf2Canvas(WPARAM wParam, LPARAM lParam);
	afx_msg void OnOpenSpecifiedSheet(WPARAM wParam, LPARAM lParam);
	afx_msg void OnInPlaceEditPart(WPARAM wParam, LPARAM lParam);

	afx_msg void OnUpdateUI_Save(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUI_SaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUI_Exit(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

private:
	// do some initializations when the application starts.
	void Init();

	// do some un-initializations when the application ends.
	void UnInit();

	// about displaying splash.
	void BeginSplash();
	void EndSplash();

private:
	CMultiDocTemplate* m_pPartDocTemplate;
	CMultiDocTemplate* m_pTaskDocTemplate;

	// whether this app will not start.
	BOOL m_bNotLaunch;

	SplashThread* m_pSplashThread;
};

extern _declspec (dllexport) ClApp cLeaderApp;
