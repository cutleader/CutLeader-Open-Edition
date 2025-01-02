#pragma once

#include "clMachineLibCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(DriverData)

BEGIN_CUTLEADER_NAMESPACE()

// this class keep the data of the driver.
class ClMachineLib_Export DriverData : public ClData
{
public:
	DriverData(void);
	~DriverData(void);

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // get/set functions.
	void SetDriName(CString strDriName) { m_strDriName = strDriName; }
	CString GetDriName() { return m_strDriName; }

	void SetDriData(LPVOID driData) { m_driData = driData; }
	LPVOID GetDriData() { return m_driData; }

	void SetDriSize(int iSize) { m_iSize = iSize; }
	int GetDriSize() { return m_iSize; }

public: // static functions.
	// the driver module name.
	static const char* GetDriModuleName() { return "DriFacade"; }

	// version file name.
	static CString GetVersionFile() { return _T("version.txt"); }

private:
	// the name of the driver.
	CString m_strDriName;

	// the driver data.
	// notes:
	//   1) dump this to disk we will get the .TAODri file.
	//   2) caller will allocate this memory.
	LPVOID m_driData;
	int m_iSize;
};

END_CUTLEADER_NAMESPACE()
