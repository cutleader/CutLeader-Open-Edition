#pragma once

#include "ClUILibCommon.h"
#include <vector>


BEGIN_CUTLEADER_NAMESPACE()

class ClUILib_Export ClWindowFinder
{
public:
	// 根据对话框模板找对话框。
	// 注：
	// 1) pWnd是GetActiveWindow()得到的窗口。
	// 2) 先看看pWnd是不是想要的，然后再递归检查pWnd的子对话框是不是想要的。找不到的话就返回空指针。
	static CWnd* FindDialogByTemplateID(CWnd* pWnd, int iDialogTemplateID);

	// 查找ChildFrame，基于文档名称。
	static CMDIChildWndEx* FindChildFrameByName(CString strName);

	// 得到所有打开文档对应的框架窗口（ChildFrame）。
	static std::vector<CMDIChildWndEx*> FindAllChildFrames();

	// 根据文档得到ChildFrame。
	static CFrameWnd* GetChildFrameByDocument(CDocument *pDoc);

	// 根据ID得到浮动面板。
	// 注：
	// 1) pWnd是GetActiveWindow()得到的窗口。
	// 2) 先看看pWnd是不是想要的，然后再递归检查pWnd的子窗口是不是想要的。找不到的话就返回空指针。
	static CWnd* FindDockPanelByID(CWnd* pWnd, int iDockPanelID);

	// 根据控件ID得到pWnd窗口中的控件。
	// 注：
	// 1）iControlID是控件ID，不能是pWnd窗口的子对话框。
	// 2) 这个函数只找pWnd窗口的下一层控件，不会递归查找。
	static CWnd* GetControlByID(CWnd* pWnd, int iControlID);

	// 得到当前视图。如果得不到，需要弹出警告。
	// 注：
	// 1) 这两个函数有点区别，1后缀的函数返回当前看到的视图区对应的视图，比如如果打开了任务（是个tab view），那么就返回板材视图。
	//    而2后缀的函数则返回任务视图，就是这个区别。
	static CView* GetCurrentView_1();
	static CView* GetCurrentView_1_WithoutDebugWarn(); // 这个函数弹出警告。
	static CView* GetCurrentView_2();

	// 得到当前文档的标签在所有标签中的索引。
	static int GetCurrentDocTabIndex();
};

END_CUTLEADER_NAMESPACE()
