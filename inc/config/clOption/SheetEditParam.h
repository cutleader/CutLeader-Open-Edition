#pragma once

#include "clOptionCommon.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

// 板材编辑参数。
class ClOption_Export SheetEditParam : public ClData
{
public:
	SheetEditParam(void);
	~SheetEditParam(void);

public: // get/set functions
	void SetMoveStep(double dMoveStep) { m_dMoveStep = dMoveStep; }
	double GetMoveStep() const { return m_dMoveStep; }

	void SetRotateStep(double dRotateStep) { m_dRotateStep = dRotateStep; }
	double GetRotateStep() const { return m_dRotateStep; }

	void EnableDistanceDetect(BOOL bDetectPartDistance) { m_bDetectPartDistance = bDetectPartDistance; }
	BOOL EnableDistanceDetect() const { return m_bDetectPartDistance; }

	void EnableAlignmentDetect(BOOL bDetectAlignment) { m_bDetectAlignment = bDetectAlignment; }
	BOOL EnableAlignmentDetect() const { return m_bDetectAlignment; }

	void EnableComDetect(BOOL bDetectCommon) { m_bDetectCommon = bDetectCommon; }
	BOOL EnableComDetect() const { return m_bDetectCommon; }

	void SetSheetMargin(double dSheetMargin) { m_dSheetMargin = dSheetMargin; }
	double GetSheetMargin() const { return m_dSheetMargin; }

	void SetPartDis(double dPartDis) { m_dPartDis = dPartDis; }
	double GetPartDis() const { return m_dPartDis; }

	void SetStickDis(int iStickDis) { m_iStickDis = iStickDis; }
	int GetStickDis() const { return m_iStickDis; }

	void SetEscapeDis(int iEscapeDis) { m_iEscapeDis = iEscapeDis; }
	int GetEscapeDis() const { return m_iEscapeDis; }

	void EnableSleekCtrl(BOOL bSleekCtrl) { m_bSleekCtrl = bSleekCtrl; }
	BOOL EnableSleekCtrl() const { return m_bSleekCtrl; }

	void SetSleekRate(double dSleekRate) { m_dSleekRate = dSleekRate; }
	double GetSleekRate() const { return m_dSleekRate; }

	void SetLoopDetectDis(int iLoopDetectDis) { m_iLoopDetectDis = iLoopDetectDis; }
	int GetLoopDetectDis() const { return m_iLoopDetectDis; }

private:
	// 步进。
	double m_dMoveStep;
	double m_dRotateStep; // 角度单位。

	BOOL m_bDetectPartDistance; // 是否进行零件距离检测。
	BOOL m_bDetectAlignment; // 是否进行对齐检测。
	BOOL m_bDetectCommon; // 一键靠碰时检测共边。
	double m_dSheetMargin;
	double m_dPartDis;
	int m_iStickDis;
	int m_iEscapeDis;

	// 布局零件时的轮廓光顺控制。
	BOOL m_bSleekCtrl; // 是否采取光顺控制。
	double m_dSleekRate; // 零件大于轮廓矩形的一定比例，不做光顺处理。

	// 工序操作时，捕获轮廓的距离。
	// 注：
	//  1) 是像素单位。
	int m_iLoopDetectDis;
};

END_CUTLEADER_NAMESPACE()
