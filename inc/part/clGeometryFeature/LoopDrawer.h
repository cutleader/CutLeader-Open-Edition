#pragma once

#include "clGeometryFeatureCommon.h"
#include "BasicGeometryDrawer.h"

DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)

BEGIN_CUTLEADER_NAMESPACE()

// this class draw the pattern loop.
// notes:
//   1) if the loop rect is within 0*0 ~ 5*5 pixel, only draw points.
//   2) if the loop rect is within 5*5 ~ 10*10 pixel, only draw circles.
class ClGeometryFeature_Export LoopDrawer : public BasicGeometryDrawer
{
public:
	LoopDrawer(GlViewPortPtr pViewPort);
	~LoopDrawer(void);

public:
	// draw pattern loop.
	void DrawPatternLoop(const IPatternLoop* pPatternLoop, Matrix2D mat = Matrix2D());
	void DrawPatternLoopList(const PatternLoopList* pPatternLoopList, Matrix2D mat = Matrix2D());

	// draw the active loop.
	void DrawActiveLoop(const IPatternLoop* pPatternLoop, COLORREF iColor, Matrix2D mat = Matrix2D());

	// draw the selected loop.
	void DrawSelectedLoop(const IPatternLoop* pPatternLoop, COLORREF iColor, Matrix2D mat = Matrix2D());

    // ��������״̬����������ν����״̬����Ը�����ѡ��״̬���ԣ�ʹ�ô������ɫ�����߿�Ϊ1��ʵ�߽��л��ơ�
    void DrawNormalStatusLoop(const IPatternLoop* pPatternLoop, COLORREF iColor, Matrix2D mat = Matrix2D());

    // �����������ݣ������߸������û�����ʽ��
    void DrawLoopData(const IPatternLoop* pPatternLoop, Matrix2D mat = Matrix2D());

	// ��ǿ���������
	void FlagOpenLoops(const PatternLoopList* pPatternLoopList);
};

END_CUTLEADER_NAMESPACE()
