#pragma once

#include "clCutSequenceUICommon.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(CutNodeList)
DECLARE_CUTLEADER_CLASS(SequenceSimulateShapeList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// “切割节点模拟器”。
// 注：
//   1) the step of starting simulation.
//       a) get the singleton object, then call "Init" to init it.
//       b) call "StartSimulation" to start.
//   2) interval time for each speed level:
//       1) 1 --- 1000 ms
//       2) 2 --- 100 ms
//       3) 3 --- 10 ms
//       4) 4 --- 1 ms
//       5) 5 --- 1 ms and 30 simulation shapes one time.
//       6) 6 --- 1 ms and 150 simulation shapes one time.
//       7) 7 --- 1 ms and 500 simulation shapes one time.
//       8) 8 --- draw all at one time.
//   3) after finishing simulation, user may want to list all cut nodes and debug each one.
//   4) now we only simulate "do-cut" sequence units in this class.
class ClCutSequenceUI_Export CutNodeSimulator
{
public:
	CutNodeSimulator(void);
	~CutNodeSimulator(void);

public:
	// get the singleton object
	static CutNodeSimulator* GetInstance();

public: // get/set functions.
	CutNodeListPtr GetCutNodeList() const { return m_pCutNodeList; }

	// get the simulate shapes which have been shown.
	SequenceSimulateShapeListPtr GetSimulatedShapes() { return m_pShownShapeList; }

	GlViewPortPtr GetDrawer() const { return m_pViewPort; }

public:
	// init the simulator.
	void Init(GlViewPortPtr pViewPort, CutNodeListPtr pCutNodeList, SequenceSimulateShapeListPtr pSeqSimShapeList);

	/************************************************************************/
	// control the simulation.                                                                      

	void StartSimulation();
	void PauseSimulation();
	void ResumeSimulation();
	void StopSimulation();

	// the speed of the simulation.
	void SpeedUp();
	void SpeedDown();
	int GetSpeed() const { return m_iSpeedLevel; }
	/************************************************************************/


	/************************************************************************/
	// get the status of the simulation.

	// check whether the simulation is finished.
	BOOL IsFinished() const { return m_bFinished; }

	// whether the simulation is paused.
	BOOL IsPaused() const { return m_bPaused; }
	/************************************************************************/


	// update the simulation shapes which will be drawn.
	// notes:
	//   1) this function will tell the view to re-draw.
	void UpdateSimShapes();

public:
	// the timer message will be sent to this function.
	static void CALLBACK TimerFunc(HWND, UINT uMsg, UINT dEvent, DWORD dwTime);

private:
	// get the time interval from the speed level.
	int GetTimeInterval();

private:
	static CutNodeSimulator* m_pCutNodeSimulator;

private:
	// 需要模拟的“切割节点”。
	CutNodeListPtr m_pCutNodeList;

	// all simulation shapes.
	SequenceSimulateShapeListPtr m_pSequenceSimulateShapeList;


	/************************************************************************/
	// use these variables, we can know where we are.

	// the index of simulation shape within "m_pSequenceSimulateShapeList".
	int m_iSimShapeIndex;

	// the simulate shapes which have been shown.
	SequenceSimulateShapeListPtr m_pShownShapeList;

	// whether the simulation is finished.
	BOOL m_bFinished;

	// whether the simulation is paused.
	BOOL m_bPaused;
	/************************************************************************/


	// the speed level.
	int m_iSpeedLevel;

	// the timer ID.
	UINT m_iTimerID;

	GlViewPortPtr m_pViewPort;
};

END_CUTLEADER_NAMESPACE()
