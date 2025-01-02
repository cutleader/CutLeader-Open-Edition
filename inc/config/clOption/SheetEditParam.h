#pragma once

#include "clOptionCommon.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

// ��ı༭������
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
	// ������
	double m_dMoveStep;
	double m_dRotateStep; // �Ƕȵ�λ��

	BOOL m_bDetectPartDistance; // �Ƿ������������⡣
	BOOL m_bDetectAlignment; // �Ƿ���ж����⡣
	BOOL m_bDetectCommon; // һ������ʱ��⹲�ߡ�
	double m_dSheetMargin;
	double m_dPartDis;
	int m_iStickDis;
	int m_iEscapeDis;

	// �������ʱ��������˳���ơ�
	BOOL m_bSleekCtrl; // �Ƿ��ȡ��˳���ơ�
	double m_dSleekRate; // ��������������ε�һ��������������˳����

	// �������ʱ�����������ľ��롣
	// ע��
	//  1) �����ص�λ��
	int m_iLoopDetectDis;
};

END_CUTLEADER_NAMESPACE()
