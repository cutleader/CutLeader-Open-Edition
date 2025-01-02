#pragma once

#include "clPartCommon.h"
#include "DataProperty.h"
#include "DataPropItem.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(ImgData)
DECLARE_CUTLEADER_CLASS(ExtPropValList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)
DECLARE_CUTLEADER_CLASS(PartCamDataList)
DECLARE_CUTLEADER_CLASS(PartItem)
DECLARE_CUTLEADER_CLASS(Part)

BEGIN_CUTLEADER_NAMESPACE()

// the part load type.
typedef enum tagPartLoadType
{
	// basic data will be loaded.
	PARTLOAD_BASIC				= 0,

	// compare with "PARTLOAD_BASIC" option, pattern data will be loaded.
	PARTLOAD_PREVIEW			= 1,

	// compare with "PARTLOAD_PREVIEW" option, loop data will be loaded.
	PARTLOAD_CADDATA			= 2,

	// compare with "PARTLOAD_CADDATA" option, CAM data will be loaded.
	PARTLOAD_ALL				= 3,
} PARTLOAD_TYPE;

// use this class to load part from database.
class ClPart_Export PartLoader
{
public:
	// load the cad data of the part.
	// notes:
	//   1) caller should set its parent ID.
	//   2) cache data in cad data will be updated.
	static PartCadDataPtr LoadPartCadData(DBConnectPtr pDBConnect, BOOL bLoadLoop = TRUE);

	// 加载预览数据。
	// 注：
	//   1) 零件没有预览数据的话，返回空值。
	static ImgDataPtr LoadPreviewData(DBConnectPtr pDBConnect);

	// 加载零件的扩展属性。
	static ExtPropValListPtr LoadExtPropVals(DBConnectPtr pDBConnect);


	/************************************************************************/
	// about load part cam data.

	// 得到零件“加工工艺”相关信息。
	// note:
	//   1) data in a DataProperty: 
	//		 1) partCamData ID.
	//		 2) expLibItem name.
	//		 3) 参数配置名称.
	//       4) expLibItem ID.
	//       5) 参数配置ID.
	static std::vector<DataProperty> GetPartCamDataProp(DBConnectPtr pDBConnect);

	// get the cam data props which lost its configuration info.
	// note:
	//   1) data in a DataProperty: 
	//		 1) partCamData ID.
	static std::vector<DataProperty> GetInvalidCamData(DBConnectPtr pDBConnect);

	// get the cam dataset prop.
	// note:
	//   1) data in a DataProperty: 
	//		 1) expLibItem name.
	//		 2) “参数配置”name.
	//       3) expLibItem ID.
	//       4) “参数配置”ID.
	//   2) if the scheme of the part is lost, those prop items will be set as invalid data.
	static BOOL GetCamDataProp(DBConnectPtr pDBConnect, LONGLONG iCamDataID, DataProperty& camDataProp);

	// load the cam data.
	/* params:
	*  1) pPartCadData: it will be used when loading cam features.
	*  2) bLoadBasic: if TRUE, do not load "m_pLoopFeatureList".
	*/
	// notes:
	//   1) the cache data of cam data will be updated.
	static PartCamDataPtr LoadPartCamData(DBConnectPtr pDBConnect, PartCadDataPtr pPartCadData, LONGLONG iCamDataID, BOOL bLoadBasic = FALSE);

	// load all CAM data of the part.
	/* params:
	*  1) bLoadBasic: if TRUE, do not load "m_pLoopFeatureList".
	*/
	static PartCamDataListPtr LoadPartCamDataList(DBConnectPtr pDBConnect, PartCadDataPtr pPartCadData, BOOL bLoadBasic = FALSE);
	/************************************************************************/


	/************************************************************************/
	// about load Part.

	// load part item from database.
	// notes: 
	//   1) if the part item is lost, return NULL.
	//   2) the cache data of returned part item will be updated.
	static PartItemPtr LoadPartItem(DBConnectPtr pDBConnect, LONGLONG iPartID, LONGLONG iPartCamDataID);

	// load part object.
	// notes:
	//   1) we do not care the expert library in this function. you know, when importing the part from .CLPrt file, we should make sure we can load cam data of the part successfully.
	static PartPtr LoadPart(DBConnectPtr pDBConnect, LONGLONG iPartID, PARTLOAD_TYPE loadType = PARTLOAD_ALL);

	// 只从零件数据库文件中加载零件，所以需传入更多的参数。
	static PartPtr LoadPart_onlyFromPartDbFile(CString strPartDbFilePath, LONGLONG iPartID, CString strName, CString strComment, LONGLONG iCreateTime, LONGLONG iModifiedTime);
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
