#pragma once

#include "clExpertLibCommon.h"
#include "DataProperty.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(ParamConfigList)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(DefaultParamConfig)
DECLARE_CUTLEADER_CLASS(MicroJointRange)
DECLARE_CUTLEADER_CLASS(MicroJointConfigItem)
DECLARE_CUTLEADER_CLASS(NCConfigTypeList)
DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(ParamConfigList)

BEGIN_CUTLEADER_NAMESPACE()

class ClExpertLib_Export ParamConfigLoader
{
public:
	/************************************************************************/
	// get basic info of the scheme.

	// �õ����������á�����Ϣ��
	// notes:
	//   1) the property item:
	//      1) ���������á� id
	//      2) ExpertLibItem id
	//      3) ���������á� name
	//      4) ExpertLibItem name
	static BOOL GetParamConfigInfo(DBConnectPtr pDBConnect, LONGLONG iConfigID, DataProperty& prop);

	// �õ���ר�ҿ�������С��������á���Ϣ��
	// ע:
	//   1) the property item:
	//      1) ���������á� id
	//      2) ExpertLibItem id
	//      3) ���������á����ơ�
	static std::vector<DataProperty> GetParamConfigList(DBConnectPtr pDBConnect, LONGLONG iExpLibItemID);

	// �ӡ��������á�ID�õ����ơ�
	// notes:
	//   1) if cannot find, return EMPTY_STRING.
	static CString GetConfigName(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);

	// ͨ�����������á����Ƶõ�ID��
	static LONGLONG GetConfigIDByName(DBConnectPtr pDBConnect, LONGLONG iExpLibItemID, CString strConfigName);
	/************************************************************************/

	/************************************************************************/
	// relative to ExpertLibItem.

	// �õ����������á������ġ�ר�ҿ����
	static LONGLONG GetExpLibItemID(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);

	// get ExpertLibItem name by scheme id
	static CString GetExpLibItemName(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);
	/************************************************************************/

	/************************************************************************/
	// ���ڼ��ء��������á���

	// ���ء��������á�����
	static ParamConfigPtr LoadParamConfig(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);

	// ���ء�ר�ҿ�������С��������á���Ϣ��
	static ParamConfigListPtr LoadParamConfigList(DBConnectPtr pDBConnect, LONGLONG iExpLibItemID, ConditionTypeListPtr pAllCondTypeList, NCConfigTypeListPtr pAllNCConfigTypeList);
	/************************************************************************/

	static DefaultParamConfigPtr LoadDflParamConfig(DBConnectPtr pDBConnect);

	// ���ء�΢���ӷ������.
	static MicroJointConfigItemPtr LoadMicroJointSetting(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);

private:
	// ���ء��������á������еġ�������������ϡ�
	static void LoadParamConfigItemList(DBConnectPtr pDBConnect, ParamConfigPtr& pParamConfig, ConditionTypeListPtr pAllCondTypeList, NCConfigTypeListPtr pAllNCConfigTypeList);

	// init the ranges of the MicroJointSetting.
	static void InitMJRanges(DBConnectPtr pDBConnect, LONGLONG iMicroJointSettingId, std::vector<MicroJointRangePtr>* pRangeList);
};

END_CUTLEADER_NAMESPACE()
