#include "stdafx.h"

#include "MasterThread.h"
#include "CmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineExec::CmdLineExec()
{
}

void CmdLineExec::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class is the program command line executive. It processes user
// command line inputs and executes them. It inherits from the command line
// command executive base class, which provides an interface for executing
// command line commands. It provides an override execute function that is
// called by a console executive when it receives a console command line input.
// The execute function then executes the command.

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("TX"))       executeTx(aCmd);
   if (aCmd->isCmd("GO1"))      executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))      executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))      executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))      executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))      executeGo5(aCmd);
   if (aCmd->isCmd("PARMS"))    executeParms(aCmd);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTx(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, "ABCD");
   int tNumBytes = strlen(aCmd->argString(1));
   gMasterThread->sendString(aCmd->argString(1));
   Prn::print(0, "send %d", tNumBytes);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

