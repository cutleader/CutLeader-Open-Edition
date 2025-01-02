#pragma once

#include "Rect2D.h"
#include "ClData.h"
#include "ParamConfigItem.h"

DECLARE_CUTLEADER_CLASS(NestConfigItem)

BEGIN_CUTLEADER_NAMESPACE()

// �Ű����
class ClExpertLib_Export NestConfigItem : public ClData,
								   virtual public ParamConfigItem
{
public:
	NestConfigItem(void);
	virtual ~NestConfigItem(void);

public: // implements IData interface.
	virtual IDataPtr Clone() const override; // ����parent id

public: // implement interface ParamConfigItem.
	PARAMCONFIG_ITEM_TYPE GetParamConfigItemType() const override { return PARAMCONFIG_ITEM_NEST; }

public: // get/set functions.
	void SetToLeft(double dToLeft) { m_dToLeft = dToLeft; }
	double GetToLeft() const { return m_dToLeft; }

	void SetToRight(double dToRight) { m_dToRight = dToRight; }
	double GetToRight() const { return m_dToRight; }

	void SetToTop(double dToTop) { m_dToTop = dToTop; }
	double GetToTop() const { return m_dToTop; }

	void SetToBottom(double dToBottom) { m_dToBottom = dToBottom; }
	double GetToBottom() const { return m_dToBottom; }

	void SetMatMargin(double dMatMargin) { m_dMatMargin = dMatMargin; }
	double GetMatMargin() const { return m_dMatMargin; }

	void EnablePartGroup(BOOL bPartGroup) { m_bEnablePartGroup = bPartGroup; }
	BOOL EnablePartGroup() const { return m_bEnablePartGroup; }

	void SetPartHullRatio(double dPartHullRatio) { m_dPartHullRatio = dPartHullRatio; }
	double GetPartHullRatio() const { return m_dPartHullRatio; }

	void SetGroupHullRatio(double dGroupHullRatio) { m_dGroupHullRatio = dGroupHullRatio; }
	double GetGroupHullRatio() const { return m_dGroupHullRatio; }

	void IsCreateGrid(BOOL bCreateGrid) { m_bCreateGrid = bCreateGrid; }
	BOOL IsCreateGrid() const { return m_bCreateGrid; }

	void SetStartCorner(RECT_CORNER startCorner) { m_startCorner = startCorner; }
	RECT_CORNER GetStartCorner() const { return m_startCorner; }

	void SetNestDirection(XY_DIRECTION iNestDir) { m_iNestDir = iNestDir; }
	XY_DIRECTION GetNestDirection() const { return m_iNestDir; }

	void SetPartDis(double dPartDis) { m_dPartDis = dPartDis; }
	double GetPartDis() const { return m_dPartDis; }

	void IsEnableComcut(BOOL bEnableComCut) { m_bEnableComCut = bEnableComCut; }
	BOOL IsEnableComcut() const { return m_bEnableComCut; }

	void IsPartInPart(BOOL bPartInPart) { m_bPartInPart = bPartInPart; }
	BOOL IsPartInPart() const { return m_bPartInPart; }

	void IsExcludeCAM(BOOL bExcludeCAM) { m_bExcludeCAM = bExcludeCAM; }
	BOOL IsExcludeCAM() const { return m_bExcludeCAM; }

	void SetPartRotStep(double dPartRotStep) { m_dPartRotStep = dPartRotStep; }
	double GetPartRotStep() const { return m_dPartRotStep; }

	void SetEvalFactor(int iEvalFactor) { m_iEvalFactor = iEvalFactor; }
	int GetEvalFactor() const { return m_iEvalFactor; }

	void OnlyUseGridNest(BOOL bOnlyUseGridNest) { m_bOnlyUseGridNest = bOnlyUseGridNest; }
	BOOL OnlyUseGridNest() const { return m_bOnlyUseGridNest; }

	void SetAllowedTime(int iAllowedTime) { m_iAllowedTime = iAllowedTime; }
	int GetAllowedTime() const { return m_iAllowedTime; }

private:
	/************************************************************************/
	// ���ϱ߾ࡣ

	// ���β��ϱ߾ࡣ
	double m_dToLeft;
	double m_dToRight;
	double m_dToTop;
	double m_dToBottom;

	// ��������ϱ߾ࡣ
	double m_dMatMargin;
	/************************************************************************/

	/************************************************************************/
	// ������顣
	// ע:
	//   1) �⼸������������ʣ������㷨��

	// whether one part can be grouped with the other part.
	BOOL m_bEnablePartGroup;

	// when the hull ratio if one part is smaller than this value, it can be grouped with the other part.
	double m_dPartHullRatio;

	// when the hull ratio if two grouped part is bigger than this value, accept the group.
	double m_dGroupHullRatio;
	/************************************************************************/

	// start corner.
	RECT_CORNER m_startCorner;

	// the nest direction.
	XY_DIRECTION m_iNestDir;

	// ������롣
	double m_dPartDis;
	BOOL m_bEnableComCut;

	// ���Ƕ�ס�
	BOOL m_bPartInPart;

	// �Ƿ��Ǽӹ����ա�
	BOOL m_bExcludeCAM;

	// �����ת������
	// ע��
	//  1) �ò����������������Ű��㷨��
	double m_dPartRotStep;

	// ��������ʱ�������ĸ���
	int m_iEvalFactor;

	// �������
	BOOL m_bOnlyUseGridNest; // �Ƿ������з�ʽ�Ű棬��ʱ���������㷨
	BOOL m_bCreateGrid; // ���з�ʽ�Ű�Ľ�����Ƿ񴴽����л��Ǵ�ɢ

	// �����Ű�ʱ��
	int m_iAllowedTime;
};

END_CUTLEADER_NAMESPACE()
