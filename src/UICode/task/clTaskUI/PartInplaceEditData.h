#pragma once

#include "ClTaskUICommon.h"

DECLARE_CUTLEADER_CLASS(PartCamData)
DECLARE_CUTLEADER_CLASS(Part)

BEGIN_CUTLEADER_NAMESPACE()

// 零件在位编辑状态。
// 注：
// 1) 在位编辑零件：是指从当前打开的任务中选择一个零件(可以从板材上选择零件，也可以从零件管理器中选择零件)，然后创建一个临时的零件，打开它进行编辑。
//    编辑完成后把正在在位编辑的零件克隆一份并另存到一个目录，然后关掉正在在位编辑的零件(即简单的关掉，不保存它)、接下来用新保存的那个零件来替换掉当前任务中的该零件。
//    如果要替换的零件已经被放到了板材上，那么替换掉板材上对应的零件放置，新零件的中心点和老零件要重合(旋转角度也一样)。对于影响到的板材，清理掉其上所有工序。
// 2) 在位编辑零件有两种场景，第一种是从板材上选择一个零件放置进行在位编辑，此时需要把它的cam数据克隆一份用来进行在位编辑。
//    第二种场景是从任务的零件列表中选一个零件进行在位编辑，此时又分两种情况，如果该零件已经放到板材上面了，那么就和第一种场景一样，
//    如果零件还未放到板材上，那么在位编辑时用的cam数据就要从Part下面复制。
// 3) 为了操作流畅，在位编辑模式下会隐藏其他的文档，只显示要在位编辑的零件。此时处于单文档模式，可以新建或打开其他的零件，
//    之前的零件会被简单的关掉，不保存。
class ClTaskUI_Export PartInplaceEditData
{
public:
	~PartInplaceEditData(void);

private:
	PartInplaceEditData(void);

public:
	static PartInplaceEditData* GetInstance();

public:
	bool GetIsUnderInPlaceEditMode() const { return m_bUnderInPlaceEditMode; }
	int GetTaskTabIndex() const { return m_iTaskTabIndex; }
	const Part* GetPart() const { return m_pPart.get(); }
	const PartCamData* GetPartCamData() const { return m_pPartCamData.get(); }

	LONGLONG GetLatestModifyTimeStamp() const { return m_iLatestModifyTimeStamp; }
	void SetLatestModifyTimeStamp(LONGLONG iLatestModifyTimeStamp) { m_iLatestModifyTimeStamp = iLatestModifyTimeStamp; }

public:
	void InitInplaceEditData(int iTaskTabIndex, const PartPtr pPart, const PartCamDataPtr pPartCamData);
	void ResetInplaceEditFlag() { m_bUnderInPlaceEditMode = false; }
	void ResetInplaceEditData(); // 该函数不处理m_bUnderInPlaceEditMode标记，只重置数据，其实就是为了释放智能指针，没有其他用途。

private:
	static PartInplaceEditData* m_pPartInplaceEditManager;

public:
	// 零件是不是处于在位编辑模式。
	bool m_bUnderInPlaceEditMode;

	// 在位编辑的零件来自这个任务。
	int m_iTaskTabIndex; // 该任务文档所在的标签页。

	// 这两个数据指明了在位编辑的对象。
	// 注：
	// 1) 这里使用智能指针是防止内存被其他地方释放了。
	PartPtr m_pPart;
	PartCamDataPtr m_pPartCamData; // 该数据可能为空，参考该类的注释。

	// 在位编辑开始时记录下零件的修改时间戳，只要这个时间戳变化了，就说明做了修改。
	LONGLONG m_iLatestModifyTimeStamp;
};

END_CUTLEADER_NAMESPACE()
