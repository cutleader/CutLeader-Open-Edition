#pragma once

#include "clOptionCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class keeps the configuration of the simulation.
// notes: 
//   1) currently, we do not provide a dialog to config these info.
class ClOption_Export SimulateInfo
{
public:
	SimulateInfo(void);
	~SimulateInfo(void);

public: // set/get functions.
	int GetSeqSimuSize() const { return m_iSeqSimuSize; }
	void SetSeqSimuSize(int iSeqSimuSize) { m_iSeqSimuSize = iSeqSimuSize; }

	int GetStartCutSimuSize() const { return m_iStartCutSimuSize; }
	void SetStartCutSimuSize(int iStartCutSimuSize) { m_iStartCutSimuSize = iStartCutSimuSize; }

	int GetMicroJointSimuSize() const { return m_iMicroJointSimuSize; }
	void SetMicroJointSimuSize(int iMicroJointSimuSize) { m_iMicroJointSimuSize = iMicroJointSimuSize; }

private:
	// the size of the triangle when simulating the sequence.
	int m_iSeqSimuSize;

	// the start cut shape size.
	int m_iStartCutSimuSize;

	// the line length when simulating the micro joint.
	int m_iMicroJointSimuSize;
};

END_CUTLEADER_NAMESPACE()
