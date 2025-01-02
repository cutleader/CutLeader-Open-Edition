#pragma once

#include "clReportTemplateCommon.h"

DECLARE_CUTLEADER_CLASS(IRptCmd)
DECLARE_CUTLEADER_CLASS(RptCmdList)

BEGIN_CUTLEADER_NAMESPACE()

// this class manage the undo/redo mechanism.
class ClReportTemplate_Export RptCmdManager
{
private:
	RptCmdManager(void);

public:
	~RptCmdManager(void);

public: // get/set functions.
	RptCmdListPtr GetUndoCommands() { return m_undoCommandList; }
	RptCmdListPtr GetRedoCommands() { return m_redoCommandList; }

public:
	static RptCmdManager* GetInstance();

	// add a command to the undo command stack.
	// notes:
	//   1) we should clear redo command stack for the same view.
	void AddCommand(IRptCmdPtr pCommand);

	// clear all undo and redo stack.
	void ClearAll();

	// redo the command.
	void Redo(IRptCmdPtr pCommand);

	// undo the command.
	void Undo(IRptCmdPtr pCommand);

private:
	// the singleton object
	static RptCmdManager *pCommandManager;

	// the undo command stack.
	RptCmdListPtr m_undoCommandList;

	// the redo command stack.
	RptCmdListPtr m_redoCommandList;
};

END_CUTLEADER_NAMESPACE()