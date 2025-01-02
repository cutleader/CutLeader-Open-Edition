#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)

BEGIN_CUTLEADER_NAMESPACE()

// �޸Ĺ���ֵ��
// ע��
// 1) ֵΪ��ʱ��Ϊ���У�ֵΪ��ʱ��Ϊȱ�ڡ�
class ClCutFeature_Export EndCutPtModifyCommand : public CommandBase
{
public:
	EndCutPtModifyCommand(EditorData editorData, LoopStartCutDataListPtr pLoopStaEndCutList, double dNewOverCut);
	~EndCutPtModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	LoopStartCutDataListPtr m_pLoopStaEndCutList;

	// �µĹ���ֵ��
	double m_dNewOverCut;

	// �ϵĹ���ֵ��
	std::vector<double> m_dOldOverCutList;

    // �����ڽ���ʱ
    std::map<LONGLONG, double> m_lineLeads_changedToZero; // ��¼һЩֱ�������ߣ���Щ���߳��ȴ�map�еķ�0ֵ���ĳ���0
    std::map<LONGLONG, double> m_arcLeads_changedToZero; // ��¼һЩԲ�������ߣ���Щ���߽Ƕȴ�map�еķ�0ֵ���ĳ���0
};

END_CUTLEADER_NAMESPACE()
