#pragma once

#include "clUtilityCommon.h"

BEGIN_CUTLEADER_NAMESPACE()


// the name of folder/files under the solution directory.
// notes:
//   1) we use following const values to rig up the full path we need.


/************************************************************************/
// for folder.

// folders under root folder.
#define OPTION_FOLDER				"option"
#define TEMP_FOLDER					"cache"
#define DATA_FOLDER					"data"
#define SAMPLE_FOLDER				"sample"
#define LOG_FOLDER					"journal"
#define NEST_LOG_FOLDER				"nestLog"
#define	HELP_FOLDER					"help"
#define	RESOURCE_FOLDER				"resource"
#define	TwoCamShare_Folder			"twoCamShare"

// folders under data folder.
#define PART_FOLDER					"part"
#define SHEET_FOLDER				"sheet"
#define TASK_FOLDER				    "Task"
#define CAMLIB_FOLDER				"camLib"
#define DB_TPL_FOLDER				"dbTpl"

// folders under help folder.
#define CHS_VIDEO_FOLDER			"chsVideo"
#define ENG_VIDEO_FOLDER			"engVideo"
#define GetStartVideo_Folder        "GetStartVideo"
/************************************************************************/


/************************************************************************/
// for file.

// about database file.
#define DB_POSTFIX					".db"
#define MAIN_DB_FILE				"main.db"
#define MACLIB_DB_FILE				"macLib.db"
#define EXPLIB_DB_FILE				"expLib.db"
#define MATLIB_DB_FILE				"matLib.db"
#define RPTLIB_DB_FILE				"rptLib.db"

// about template database file.
#define PART_TPL_DB_FILE			"partTpl.db"
#define PART_EXP_DB_FILE			"prtExpInfo.db"
#define SHEET_TPL_DB_FILE			"sheetTpl.db"
#define SHEET_EXP_DB_FILE			"shtExpInfo.db"
#define TASK_TPL_DB_FILE			"TaskTemplate.db"
#define TaskExport_DbFile			"TaskExportInfo.db"
#define MACLIB_TPL_DB_FILE			"macLibTpl.db"
#define EXPLIB_EXP_DB_FILE			"expLibExpInfo.db"
#define EXPLIB_TPL_DB_FILE			"expLibTpl.db"
#define MATLIB_EXP_DB_FILE			"matLibExpInfo.db"
#define MATLIB_TPL_DB_FILE			"matLibTpl.db"
#define DB_EXP_DB_FILE				"dbExpInfo.db"

// about config file.
#define LOG_CONFIG_FILE				"log.properties"
#define ADV_OPTION_FILE				"advOption.xml"
#define USER_OPTION_FILE			"userOption.xml"
#define NEST_OPTION_FILE			"nestorOption.xml"
#define TPLDESIGN_CONFIG_FILE		"tplDesign.xml"

// resource file.
#define CL_RES_FILE				    "clResource.dll"
#define TRANSLATION_LIB_FILE		"translationLib.xlsx"

// some exe file.
#define CAM_EXE_FILE				"clApp.exe"
#define KB_CONFIG_EXE_FILE			"clKnowledgeBaseConfig.exe"
#define DATA_BK_EXE_FILE			"clDataBackuper.exe"
#define TPL_DESIGN_EXE_FILE			"clTemplateDesign.exe"

// the log file.
#define DB_BK_DATE_FILE				"dbBkDate.log"
#define LOG_FILE					"event.log"

// help file
#define HELP_CHS_FILE				"Help_CHS.chm"
#define HELP_ENG_FILE				"Help_ENG.chm"
#define	VIDEO_LIST_FILE				"videoList.xml"
/************************************************************************/


END_CUTLEADER_NAMESPACE()
