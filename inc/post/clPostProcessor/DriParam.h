#pragma once

#include <string>
#include <vector>

namespace CutLeader
{

// these data structures will be used to send/receive data with driver.
// notes:
//   1) this file do not include any head file of the system, this can make this file clean.
//   2) we use "int" type to replace enum/union, i think this is easier even though that's not reasonable.
//      a) the data type, 1:int, 2:double, 3:string, 4:BOOL.
//      b) the direction, 1:X, 2:Y.
//		c) the corner, 1:left top, 2:left bottom, 3: right bottom, 4:right top.

/************************************************************************/
// the type only for test.

struct TestType
{
	int iTmp;
	std::string strTmp;

	std::vector<std::string> strList;
};
/************************************************************************/

/************************************************************************/
// use these type to init the driver.

// 64 bits int.
struct LongInt
{
	long long int llLongInt;
};

// part infos within the sheet.
struct PartInfoList
{
	// part id
	std::vector<std::string> strPartIDList;

	// the part name.
	std::vector<std::string> strPartNameList;

	// the part size.
	std::vector<double> dPartHeightList;
	std::vector<double> dPartWidthList;

	// the part count.
	std::vector<int> iPartCountList;
};

// sheet info.
struct SheetInfo
{
	// the sheet name
	std::string strSheetname;

	// the sheet size.
	double dSheetHeight;
	double dSheetWidth;

	// the sheet thickness.
	double dThickness;

	// iron, steel,...
	std::string strMaterial;
};

// machineInfo
struct MachineInfo
{
	// the machine name.
	std::string strMacName;

	// the table size.
	double dTableLength;
	double dTableWidth;

	// the positions of the clamps.
	std::vector<double> clampPosList;
};

// NC config item.
// notes: we do not use union to wrapper the NC config.
struct NCConfigItemList
{
	// driver will use this ID to recognize the NC config.
	std::vector<int> iUniqueIDList;

	// the data type.(1:int, 2:double, 3:string, 4:BOOL)
	std::vector<int> iDataTypeList;

	// the value, only one variable will be set according to "iDataType".
	std::vector<int> iValList;
	std::vector<double> dValList;
	std::vector<std::string> strValList;
	std::vector<int> bValList;
};
/************************************************************************/

/************************************************************************/
// types for outputting NC code.

// the result info of the each request, from this structure we can know whether we succeed, otherwise we can get the error code.
struct RET_INFO
{
	// the result code.
	// 0: succeed.
	// 1: do not support sub-routine, we should explode to basic cut nodes.
	// 2: for sub-routine, we should generate the sub-routine template.
	// 3: for grid-cut, if do not support grid-cut but support sub-rt, we should explode it.
	// 4: if memory is not enough,  use this.
	// 100: py runtime error.
	int iRetCode;

	// the debug message from the driver, driver can send any message to c++ side to let me know what happened.
	std::string strDbgMsg;
};

// point.
struct PtParam
{
	double dX;
	double dY;
};

// NC section.
struct NCSection
{
	// the ID of the NC row, driver should generate the ID.
	std::vector<int> iNCRowIDList;

	// rows of NC code.
	std::vector<std::string> strNCRowList;

	// we put these NC row into these cans.
	std::vector<int> iCanIdxList;
};

// condition Info.
// notes: similar with "NCConfigItemList".
struct CondInfoList
{
	// driver will use this ID to recognize the condition type.
	std::vector<int> iUniqueIDList;

	// the data type.(0:string, 1:int, 2:bool, 3:double)
	std::vector<int> iDataTypeList;

	// the value, only one variable will be set according to "iDataType".
	std::vector<int> iValList;
	std::vector<double> dValList;
	std::vector<std::string> strValList;
	std::vector<int> bValList;
};

// arc info.
struct ArcInfo
{
	// center pt.
	PtParam centerPt;

	// start pt.
	PtParam startPt;

	// end pt.
	PtParam endPt;

	// radius
	double dRadius;

	// start angle.
	double dStartAngle;

	// sweep angle.
	double dSweepAngle;
};

// 刀具信息
struct ToolInfo
{
    std::string strToolName;
    double dToolSize;
};

// 工序轮廓信息，参加该类cutnode的文档
struct SeqLoopInfo
{
	int iBoundaryLoop;
	std::vector<std::string> strPartIDList;

    // 工序类型
    int iSequenceType;

    // 如果是共边工序，记录其共边组ID
    std::string strCommonCutGroupName;
};
/************************************************************************/

}