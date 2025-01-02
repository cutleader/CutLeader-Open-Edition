#pragma once

#include "cutLeaderCommon.h"

#if defined(ClPostProcessor_Home)
#    define ClPostProcessor_Export _declspec(dllexport)
#else
#    define ClPostProcessor_Export _declspec(dllimport)
#endif


namespace CutLeader
{
	struct PartInfoList;
	typedef boost::shared_ptr<PartInfoList> PartInfoListPtr;
	struct SheetInfo;
	typedef boost::shared_ptr<SheetInfo> SheetInfoPtr;
	struct MachineInfo;
	typedef boost::shared_ptr<MachineInfo> MachineInfoPtr;
    struct ToolInfo;
    typedef boost::shared_ptr<ToolInfo> ToolInfoPtr;
	struct SeqLoopInfo;
	typedef boost::shared_ptr<SeqLoopInfo> SeqLoopInfoPtr;
	struct ArcInfo;
	typedef boost::shared_ptr<ArcInfo> ArcInfoPtr;
	struct CondInfoList;
	typedef boost::shared_ptr<CondInfoList> CondInfoListPtr;
	struct NCConfigItemList;
	typedef boost::shared_ptr<NCConfigItemList> NCConfigItemListPtr;
	struct NCSection;
	typedef boost::shared_ptr<NCSection> NCSectionPtr;
}
using namespace CutLeader;