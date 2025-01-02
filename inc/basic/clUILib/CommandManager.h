#pragma once

#include "clUILibCommon.h"

DECLARE_CUTLEADER_CLASS(CommandList)
DECLARE_CUTLEADER_CLASS(ICommand)

BEGIN_CUTLEADER_NAMESPACE()


// this class manage the undo/redo mechanism.
// notes:
//   1) this class hold all commands which have been executed. when activate a view, the view will find the relative commands from here.
//   2) the undo/redo mechanism.
//      a) the space width between each command means nothing.
//      b) you can see after redo, the commands for view1 are not together anymore, but it doesn't matter because the order of them do not change.
//      c) we always add/remove the command from the stack top regardless of undo/redo.
/*                                                                            
   ***** *   *   *   *        *   *   *   *         *   *   *   *                              
         *   *   *   *        *   *   *   *         *   *   *   *            * stack                
         *c11*   *   *        *c11*   *c22*         *c11*   *   *            * bottom           
   view1 *   *   *   *        *   *   *   *         *   *   *   *            *                 
         *   *   *   *        *   *   *   *         *   *   *   *            *                 
         *c12*   *   *        *   *   *   *         *   *   *   *            *                 
         *   *   *   *        *   *   *   *         *   *   *   *            *                 
   ***** *   *   *   *        *   *   *   *         *   *   *   *            *                 
         *   *   *   *        *   *   *c12*         *   *   *   *            *                 
         *   *   *   *        *   *   *   *         *   *   *   *            *                 
         *c21*   *   *        *c21*   *   *         *c21*   *   *            *                 
   view2 *   *   *   *        *   *   *   *         *   *   *   *            *                 
         *   *   *   *        *   *   *   *         *   *   *   *            *                 
         *c22*   *   *        *   *   *   *         *   *   *   *            *                 
         *   *   *   *        *   *   *   *         *   *   *   *            *  push
   ***** *   *   *   *        *   *   *   *         *   *   *   *            *  dir           
         *   *   *   *        *   *   *   *         *   *   *   *            *                 
         *   *   *   *        *   *   *   *         *   *   *   *            *                 
         *c31*   *   *        *c31*   *   *         *c31*   *   *         *  *  *              
   view3 *   *   *   *        *   *   *   *         *   *   *   *         *  *  *              
         *   *   *   *        *   *   *   *         *   *   *   *          * * *               
         *c32*   *   *        *c32*   *   *         *c32*   *   *          * * *               
         *   *   *   *        *   *   *   *         *   *   *   *          * * *               
   ***** *   *   *   *        *   *   *   *         *   *   *   *           ***                
         *   *   *   *        *   *   *   *         *   *   *   *           ***                
         *   *   *   *        *   *   *   *         *   *   *   *           ***                
         *   *   *   *        *   *   *   *         *c12*   *   *            *                 
         *   *   *   *        *   *   *   *         *c22*   *   *              stack                
         *   *   *   *        *   *   *   *         *   *   *   *              top

		    begin           after undo c22,c12    after redo c12,c22 
*/
class ClUILib_Export CommandManager
{
private:
	CommandManager(void);

public:
	~CommandManager(void);

public: // get/set functions.
	CommandListPtr GetUndoCommands() { return m_pUndoCmdList; }
	CommandListPtr GetRedoCommands() { return m_pRedoCmdList; }

public:
	static CommandManager* GetInstance();

	// add a command to the undo command stack.
	// notes:
	//   1) we should clear redo command stack for the same view.
	void AddCommand(ICommandPtr pCommand);

	// clear all undo and redo stack.
	void ClearAll();

	// redo the command.
	void Redo(ICommandPtr pCommand);

	// undo the command.
	void Undo(ICommandPtr pCommand);

private:
	// the singleton object
	static CommandManager *pCommandManager;

	// the undo command stack.
	CommandListPtr m_pUndoCmdList;

	// the redo command stack.
	CommandListPtr m_pRedoCmdList;
};

END_CUTLEADER_NAMESPACE()