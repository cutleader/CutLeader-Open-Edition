#include "StdAfx.h"
#include "WrongToolSizeLoopInstances.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "LoopInstanceList.h"
#include "PartLayoutHelper.h"
#include "LoopInstance.h"
#include "GlViewPort.h"
#include "LoopDrawer.h"
#include "LoopInstanceCache.h"


using namespace std;


BEGIN_CUTLEADER_NAMESPACE()


WrongToolSizeLoopInstances::WrongToolSizeLoopInstances()
{
    m_pLoopInstances_wrongToolSize.reset(new LoopInstanceList);
}

WrongToolSizeLoopInstances::~WrongToolSizeLoopInstances()
{

}

void WrongToolSizeLoopInstances::Init(const LoopInstanceList* pAllLoopInstances, const std::set<LONGLONG>& patternLoops_failedToOffset)
{
    m_pLoopInstances_wrongToolSize->clear();
    for (unsigned int i = 0; i < pAllLoopInstances->size(); i++)
    {
        LoopInstancePtr pLoopInstance = pAllLoopInstances->operator [](i);
        LONGLONG iPatternLoopID = pLoopInstance->GetPatternLoopID();
        if (patternLoops_failedToOffset.find(iPatternLoopID) != patternLoops_failedToOffset.end())
        {
            m_pLoopInstances_wrongToolSize->push_back(pLoopInstance);
        }
    }
}

void WrongToolSizeLoopInstances::Update(const LoopInstanceList* pLoopInstances_willBeChecked, double dToolSize)
{
    map<LONGLONG, bool> checkResults; // 缓存检查的结果
    for (unsigned int i = 0; i < pLoopInstances_willBeChecked->size(); i++)
    {
        LoopInstancePtr pLoopInstance = pLoopInstances_willBeChecked->operator [](i);

        // 先看看是不是已经测试过该几何轮廓。
        map<LONGLONG, bool>::const_iterator iter = checkResults.find(pLoopInstance->GetPatternLoopID());
        if (iter == checkResults.end())
        {
            bool bRet = PartLayoutHelper::CheckToolSize_4_LoopInstance(pLoopInstance.get(), dToolSize);
            if (bRet) // 该轮廓分配了合适的刀具尺寸。
            {
                m_pLoopInstances_wrongToolSize->DeleteLoopInstanceByPatternLoopID(pLoopInstance->GetPatternLoopID());
            }
            else
            {
                m_pLoopInstances_wrongToolSize->push_back(pLoopInstance);
            }
            checkResults[pLoopInstance->GetPatternLoopID()] = bRet;
        }
        else
        {
            if (!iter->second)
            {
                m_pLoopInstances_wrongToolSize->push_back(pLoopInstance);
            }
        }
    }
}

void WrongToolSizeLoopInstances::Draw(GlViewPortPtr pViewPort)
{
    DisplayParamPtr pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam();
    LoopDrawer loopDrawer(pViewPort);

    // backup the old draw config.
    ROP_MODE ropMode;
    pViewPort->GetROP(ropMode);
    COLORREF dwOldColor;
    pViewPort->GetDrawColor(dwOldColor);

    // config the drawer.
    pViewPort->SetDrawColor(*pDisplayParam->GetIncorrectToolSizeColor());
    pViewPort->SetROP(ROP_COPY);

    // draw.
    for (unsigned int i = 0; i < m_pLoopInstances_wrongToolSize->size(); i++)
    {
        LoopInstancePtr pLoopInstance = m_pLoopInstances_wrongToolSize->at(i);
        loopDrawer.DrawLoopData(pLoopInstance->GetCacheData()->GetPatternLoop().get(), pLoopInstance->GetTransformMatrix());
    }

    // restore the drawer
    pViewPort->SetROP(ropMode);
    pViewPort->SetDrawColor(dwOldColor);
}

END_CUTLEADER_NAMESPACE()
