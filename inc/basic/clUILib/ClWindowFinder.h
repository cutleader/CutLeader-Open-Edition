#pragma once

#include "ClUILibCommon.h"
#include <vector>


BEGIN_CUTLEADER_NAMESPACE()

class ClUILib_Export ClWindowFinder
{
public:
	// ���ݶԻ���ģ���ҶԻ���
	// ע��
	// 1) pWnd��GetActiveWindow()�õ��Ĵ��ڡ�
	// 2) �ȿ���pWnd�ǲ�����Ҫ�ģ�Ȼ���ٵݹ���pWnd���ӶԻ����ǲ�����Ҫ�ġ��Ҳ����Ļ��ͷ��ؿ�ָ�롣
	static CWnd* FindDialogByTemplateID(CWnd* pWnd, int iDialogTemplateID);

	// ����ChildFrame�������ĵ����ơ�
	static CMDIChildWndEx* FindChildFrameByName(CString strName);

	// �õ����д��ĵ���Ӧ�Ŀ�ܴ��ڣ�ChildFrame����
	static std::vector<CMDIChildWndEx*> FindAllChildFrames();

	// �����ĵ��õ�ChildFrame��
	static CFrameWnd* GetChildFrameByDocument(CDocument *pDoc);

	// ����ID�õ�������塣
	// ע��
	// 1) pWnd��GetActiveWindow()�õ��Ĵ��ڡ�
	// 2) �ȿ���pWnd�ǲ�����Ҫ�ģ�Ȼ���ٵݹ���pWnd���Ӵ����ǲ�����Ҫ�ġ��Ҳ����Ļ��ͷ��ؿ�ָ�롣
	static CWnd* FindDockPanelByID(CWnd* pWnd, int iDockPanelID);

	// ���ݿؼ�ID�õ�pWnd�����еĿؼ���
	// ע��
	// 1��iControlID�ǿؼ�ID��������pWnd���ڵ��ӶԻ���
	// 2) �������ֻ��pWnd���ڵ���һ��ؼ�������ݹ���ҡ�
	static CWnd* GetControlByID(CWnd* pWnd, int iControlID);

	// �õ���ǰ��ͼ������ò�������Ҫ�������档
	// ע��
	// 1) �����������е�����1��׺�ĺ������ص�ǰ��������ͼ����Ӧ����ͼ������������������Ǹ�tab view������ô�ͷ��ذ����ͼ��
	//    ��2��׺�ĺ����򷵻�������ͼ�������������
	static CView* GetCurrentView_1();
	static CView* GetCurrentView_1_WithoutDebugWarn(); // ��������������档
	static CView* GetCurrentView_2();

	// �õ���ǰ�ĵ��ı�ǩ�����б�ǩ�е�������
	static int GetCurrentDocTabIndex();
};

END_CUTLEADER_NAMESPACE()
