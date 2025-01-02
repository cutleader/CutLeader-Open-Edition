#pragma once

#include "clUtilityCommon.h"
#include "clMachineLibCommon.h"
#include "baseConst.h"
#include "ClData.h"

// 几个常用的unique ID。
#define CUT_SPEED			1800 // 公制系统下是mm/min，英制系统下是inch/min。
#define RAPID_SPEED			1801 // 公制系统下是mm/min，英制系统下是inch/min。
#define PIERCE_TIME			1802 // 单位是秒

#define CUT_COST			1803 // 切割费用，浮点数，单位 价格/每米 (Price/Per_Meter)
#define RAPIDMOVE_COST		1804 // 快速移动费用，浮点数，单位 价格/每米(Price/Per_Meter)
#define PIERCE_COST			1805 // 穿孔费用，浮点数，单位 价格/每次(Price/Per_Pierce)
//

DECLARE_CUTLEADER_CLASS(NCConfigType)

BEGIN_CUTLEADER_NAMESPACE()

class ClMachineLib_Export NCConfigType : public ClData
{
public:
	NCConfigType(void);
	NCConfigType(CString ncConfigName, BASIC_DATA_TYPE ncConfigDataType = BASIC_DATA_INT, CString comment = EMPTY_STRING);
	~NCConfigType(void);

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // get/set functions.
	void SetNCConfigName(CString ncConfigName) { m_ncConfigName = ncConfigName; }
	CString GetNCConfigName() { return m_ncConfigName; }

	void SetNCConfigDataType(BASIC_DATA_TYPE ncConfigDataType) { m_ncConfigDataType = ncConfigDataType; }
	BASIC_DATA_TYPE GetNCConfigDataType() { return m_ncConfigDataType; }

	void SetUniqueID(int iUniqueID) { m_iUniqueID = iUniqueID; }
	int GetUniqueID() { return m_iUniqueID; }

	void SetComment(CString comment) { m_comment = comment; }
	CString GetComment() { return m_comment; }

public:
	// get the "string" name of the data type.
	// e.g., return "String","Integer",...
	CString GetDataTypeName();

	// parse the condition type from string
	void SetDataType(CString str);

public:
	static StringListPtr GetAllDataType();

private:
	void CommonConstruct();

private:
	// the name.
	CString m_ncConfigName;

	// the data type.
	BASIC_DATA_TYPE m_ncConfigDataType;

	// this ID will be used by driver.
	int m_iUniqueID;

	// the comment of the condition.
	CString m_comment;
};

END_CUTLEADER_NAMESPACE()
