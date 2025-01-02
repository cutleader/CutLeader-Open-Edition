#pragma once

#include "clBaseDataCommon.h"

BEGIN_CUTLEADER_NAMESPACE()


// the result of the license checking, ���֤��֤�Ľ����
enum LicenseCheckResultType
{
    // integraty check failed, the kernel dlls should be loaded together, �����Լ��ʧ�ܣ�kernel������dll��Ҫ��һ����ء�
    LicenseCheckResult_integratyFail            = 0,

    // no valid license, please contact TAOSoft to buy the license.
    LicenseCheckResult_noLicense                = 1,

    // the license is expired.
    LicenseCheckResult_dateExpired              = 2,

    // standard edition
    LicenseCheckResult_stdEdition               = 3,

    // professional edition
    LicenseCheckResult_professionalEdition      = 4,

    // ultimate edition
    LicenseCheckResult_ultimateEdition          = 5,
};


// the result of updating the dongle, ���¼��ܹ�����Ȩ��Ϣ�Ľ��.
enum UpdateDongleResultType
{
    // �ɹ�
    UpdateDongleResultType_Succeed              = 0,

    // fail to write the dongle.
    UpdateDongleResultType_FailToWrite          = 1,

    // dongle not attached.
    UpdateDongleResultType_DongleNotAttached    = 2,

    // key file error.
    UpdateDongleResultType_KeyFileError         = 3,
};


// this class will verify the license of cutLeader, �����������֤CutLeader����Ȩ��
class ClBaseData_Export LicenseManager
{
public:
    // check license.
    static LicenseCheckResultType CheckLicense();

    // check license, if cutleader v6 dongle is attached, more info will be returned.
    static LicenseCheckResultType CheckLicense_2(int& iProductID, int& iCustomerID, CTime& startTime, CTime& endTime);

    // update license to the dongle.
    static UpdateDongleResultType UpdateDongle(CString strKeyFilePath);
};

END_CUTLEADER_NAMESPACE()
