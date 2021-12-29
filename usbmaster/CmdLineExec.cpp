#include "stdafx.h"

#include "someUSBHostParms.h"
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
   if (aCmd->isCmd("B1"))       gMasterThread1->mBFlag = aCmd->argBool(1);
   if (aCmd->isCmd("B2"))       gMasterThread2->mBFlag = aCmd->argBool(1);

   if (aCmd->isCmd("SEND1"))    executeSend1(aCmd);
   if (aCmd->isCmd("SEND2"))    executeSend2(aCmd);

   if (aCmd->isCmd("TEST1"))    executeTest1(aCmd);
   if (aCmd->isCmd("TEST2"))    executeTest2(aCmd);

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

void CmdLineExec::executeSend1(Ris::CmdLineCmd* aCmd)
{
   char tString[100];
   if (aCmd->numArg() == 0)
   {
      strcpy(tString, "ABCDEFGH\n");
   }
   else
   {
      sprintf(tString, "%s\n", aCmd->argWhole());
      my_string_toupper(tString);
   }
   gMasterThread1->sendString(tString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeSend2(Ris::CmdLineCmd* aCmd)
{
   char tString[100];
   if (aCmd->numArg() == 0)
   {
      strcpy(tString, "ABCDEFGH\n");
   }
   else
   {
      sprintf(tString, "%s\n", aCmd->argWhole());
      my_string_toupper(tString);
   }
   gMasterThread2->sendString(tString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTest1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 128);
   gMasterThread1->sendTestBytes(aCmd->argInt(1));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTest2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 128);
   gMasterThread2->sendTestBytes(aCmd->argInt(1));
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
   Some::gUSBHostParms.reset();
   Some::gUSBHostParms.readSection("default");
   Some::gUSBHostParms.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

