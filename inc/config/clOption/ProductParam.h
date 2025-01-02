#pragma once

#include "clUILibCommon.h"
#include "clOptionCommon.h"
#include "ClData.h"
#include "MultiLanguageMgr.h"


BEGIN_CUTLEADER_NAMESPACE()

// ��Ʒ�������á�
class ClOption_Export ProductParam : public ClData
{
public:
	ProductParam(void);
	~ProductParam(void);

public: // get/set functions.
	void SetDimUnitType(DIM_UNIT_TYPE dimUnit) { m_dimUnit = dimUnit; }
	DIM_UNIT_TYPE GetDimUnitType() const { return m_dimUnit; }

	void SetLang(LANG_TYPE lang) { m_lang = lang; }
	LANG_TYPE GetLang() const { return m_lang; }

	void SetBkLocation(CString strBkLocation) { m_strBkLocation = strBkLocation; }
	CString GetBkLocation() const { return m_strBkLocation; }

	void SetDBBackupDay(int iDBBackupDay) { m_iDBBackupDay = iDBBackupDay; }
	int GetDBBackupDay() const { return m_iDBBackupDay; }

	void SetLogBackupSize(int iLogBackupSize) { m_iLogBackupSize = iLogBackupSize; }
	int GetLogBackupSize() const { return m_iLogBackupSize; }

	void IsUseInside(BOOL b) { m_bInside = b; }
	BOOL IsUseInside() const { return m_bInside; }

    void IsEnableReset4ChainSeq(BOOL bEnableReset4ChainSeq) { m_bEnableReset4ChainSeq = bEnableReset4ChainSeq; }
    BOOL IsEnableReset4ChainSeq() const { return m_bEnableReset4ChainSeq; }

	void SetIsRunAsBackgroundApp(BOOL b) { m_bRunAsBackgroundApp = b; }
	BOOL GetIsRunAsBackgroundApp() const { return m_bRunAsBackgroundApp; }

private:
	// the dimension unit.
	DIM_UNIT_TYPE m_dimUnit;

	// the language.
	LANG_TYPE m_lang;

	// the backup location.
	CString m_strBkLocation;

	// days interval for backup database.
	int m_iDBBackupDay;

	// size limitation for backup log file.
	int m_iLogBackupSize;

	// whether the product is used internal.
	BOOL m_bInside;

    // ���ƹ��ܣ��Ƿ����������ĸ�λ����
    BOOL m_bEnableReset4ChainSeq;

	// SenfengCAM�ǲ����Ժ�̨����ķ�ʽ���С�
	BOOL m_bRunAsBackgroundApp;
};

END_CUTLEADER_NAMESPACE()
