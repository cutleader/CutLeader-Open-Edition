#include "StdAfx.h"
#include "CutNodeSimulator.h"

#include "GlViewPort.h"

#include "LineArc2DList.h"
#include "Triangle2D.h"

#include "ICutPartSequence.h"
#include "ICutSequence.h"
#include "CutSequenceList.h"
#include "CutNode.h"
#include "CutNodeList.h"
#include "SequenceSimulateShapeList.h"
#include "ISequenceSimulateShape.h"
#include "CutNodeViewBar.h"

#include "CLOptionInfo.h"
#include "ConcealConfig.h"
#include "DebugUIInfo.h"
#include "DisplayParam.h"
#include "SimulateInfo.h"
#include "PartPlacementEditAction.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CutNodeSimulator* CutNodeSimulator::m_pCutNodeSimulator = NULL;

CutNodeSimulator* CutNodeSimulator::GetInstance()
{
	if (m_pCutNodeSimulator == NULL)
		m_pCutNodeSimulator = new CutNodeSimulator();

	return m_pCutNodeSimulator;
}

CutNodeSimulator::CutNodeSimulator(void)
{
	m_pShownShapeList.reset(new SequenceSimulateShapeList);
	m_bFinished = TRUE;
}

CutNodeSimulator::~CutNodeSimulator(void)
{
}

void CutNodeSimulator::Init(GlViewPortPtr pViewPort, CutNodeListPtr pCutNodeList, SequenceSimulateShapeListPtr pSeqSimShapeList)
{
	m_pViewPort = pViewPort;
	m_pCutNodeList = pCutNodeList;
	m_pSequenceSimulateShapeList = pSeqSimShapeList;

	// init some state.
	m_iSimShapeIndex = 0;
	m_bFinished = FALSE;
	m_bPaused = FALSE;
	m_pShownShapeList->clear();

	// init the sequence simulation speed.
	DisplayParamPtr pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam();
	m_iSpeedLevel = pDisplayParam->GetSpeed();
}

void CutNodeSimulator::StartSimulation()
{
	m_iTimerID = ::SetTimer(NULL, 0, GetTimeInterval(), (TIMERPROC)CutNodeSimulator::TimerFunc); 
}

void CutNodeSimulator::StopSimulation()
{
	::KillTimer(NULL, m_iTimerID);

	// reset some state.
	m_pShownShapeList->clear(); // 把动态模拟的形状清除。
	m_bFinished = TRUE;
	m_bPaused = FALSE;
}

void CutNodeSimulator::PauseSimulation()
{
	::KillTimer(NULL, m_iTimerID);
	m_bPaused = TRUE;
}

void CutNodeSimulator::ResumeSimulation()
{
	m_iTimerID = ::SetTimer(NULL, 0, GetTimeInterval(), (TIMERPROC)CutNodeSimulator::TimerFunc); 
	m_bPaused = FALSE;
}

void CutNodeSimulator::SpeedUp()
{
	if (m_iSpeedLevel < 7)
	{
		m_iSpeedLevel += 1;

		// reset the timer.
		KillTimer(NULL, m_iTimerID);
		m_iTimerID = ::SetTimer(NULL, 0, GetTimeInterval(), (TIMERPROC)CutNodeSimulator::TimerFunc); 
	}
}

void CutNodeSimulator::SpeedDown()
{
	if (m_iSpeedLevel > 1)
	{
		m_iSpeedLevel -= 1;

		// reset the timer.
		KillTimer(NULL, m_iTimerID);
		m_iTimerID = ::SetTimer(NULL, 0, GetTimeInterval(), (TIMERPROC)CutNodeSimulator::TimerFunc); 
	}
}

void CutNodeSimulator::UpdateSimShapes()
{
	if ((unsigned int)m_iSimShapeIndex < m_pSequenceSimulateShapeList->size())
	{
		if (m_iSpeedLevel == 5 || m_iSpeedLevel == 6 || m_iSpeedLevel == 7)
		{
			// the interval count.
			int iIntervalNum;
			if (m_iSpeedLevel == 5)
				iIntervalNum = 30;
			else if (m_iSpeedLevel == 6)
				iIntervalNum = 150;
			else if (m_iSpeedLevel == 7)
				iIntervalNum = 500;

			int iLeftCount = m_pSequenceSimulateShapeList->size() - m_iSimShapeIndex;
			SequenceSimulateShapeList::iterator beginIter = m_pSequenceSimulateShapeList->begin() + m_iSimShapeIndex;
			if (iLeftCount >= iIntervalNum)
			{
				SequenceSimulateShapeList::iterator endIter = beginIter + iIntervalNum;
				m_pShownShapeList->insert(m_pShownShapeList->end(), beginIter, endIter);
				m_iSimShapeIndex += iIntervalNum;
			}
			else
			{
				SequenceSimulateShapeList::iterator endIter = beginIter + iLeftCount;
				m_pShownShapeList->insert(m_pShownShapeList->end(), beginIter, endIter);
				m_iSimShapeIndex += iLeftCount;
			}
		}
		else if (m_iSpeedLevel == 8)
		{
			for (unsigned int i = m_iSimShapeIndex; i < m_pSequenceSimulateShapeList->size(); i++)
				m_pShownShapeList->push_back(m_pSequenceSimulateShapeList->at(i));

			// here the size will be set.
			m_iSimShapeIndex = m_pSequenceSimulateShapeList->size();
		}
		else
		{
			ISequenceSimulateShapePtr pSeqSimShape = m_pSequenceSimulateShapeList->at(m_iSimShapeIndex);
			m_pShownShapeList->push_back(pSeqSimShape);
			m_iSimShapeIndex++;
		}
	}
	else
	{
		// the simulation is finished.
		StopSimulation();
	}

	// told the view to redraw.
	CWnd* pView = CWnd::FromHandle(m_pViewPort->GetWnd());
	pView->Invalidate();
}

void CutNodeSimulator::TimerFunc(HWND, UINT uMsg, UINT dEvent, DWORD dwTime)
{
	CutNodeSimulator* pCutNodeSimulator = GetInstance();

	// figure out the nest simulation shape and told the view to draw.
	pCutNodeSimulator->UpdateSimShapes();

    // 模拟结束后，进入零件选择动作。
    if (pCutNodeSimulator->IsFinished())
    {
        PartPlacementEditAction::PostMessage2EnterPartPlacementSelectAction();
    }

	// after finishing simulation, check whether want to list all cut nodes.
	ConcealConfigPtr pConcealConfig = CLOptionInfo::GetInstance()->GetConcealConfig();
	DebugUIInfoPtr pDebugUIInfo = pConcealConfig->GetDebugUIInfo();
	if (pDebugUIInfo->IsShowCutNode() && pCutNodeSimulator->IsFinished())
	{
		CutNodeViewBar* pCutNodeViewBar = CutNodeViewBar::GetInstance();
		pCutNodeViewBar->InitBar(pCutNodeSimulator->GetCutNodeList(), pCutNodeSimulator->GetDrawer());
		if (!pCutNodeViewBar->IsVisible())
			pCutNodeViewBar->ShowPane(TRUE, FALSE, TRUE);
	}
}

int CutNodeSimulator::GetTimeInterval()
{
	int iInterval;

	if (m_iSpeedLevel == 1)
		iInterval = 1000;
	else if (m_iSpeedLevel == 2)
		iInterval = 100;
	else if (m_iSpeedLevel == 3)
		iInterval = 10;
	else if (m_iSpeedLevel >= 4)
		iInterval = 1;

	return iInterval;
}

END_CUTLEADER_NAMESPACE()
