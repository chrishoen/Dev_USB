#include "stdafx.h"

#include "risThreadsProcess.h"
#include "MainInit.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "someUSBHostParms.h"
#include "MasterThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Begin program.

   main_initialize(argc, argv);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Launch program threads.

   if (Some::gUSBHostParms.mEnable1)
   {
      gMasterThread1 = new MasterThread(1);
      gMasterThread1->launchThread();
   }
   if (Some::gUSBHostParms.mEnable2)
   {
      gMasterThread2 = new MasterThread(2);
      gMasterThread2->launchThread();
   }


   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Show program threads.

   Ris::Threads::showCurrentThreadInfo();
   if (gMasterThread1)    gMasterThread1->showThreadInfo();
   if (gMasterThread2)    gMasterThread2->showThreadInfo();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Execute console command line executive, wait for user to exit.

   CmdLineExec* tExec = new CmdLineExec;
   Ris::gCmdLineConsole.execute(tExec);
   delete tExec;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Shutdown program threads.

   if (gMasterThread1)     gMasterThread1->shutdownThread();
   if (gMasterThread2)     gMasterThread2->shutdownThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Delete program threads.

   if (gMasterThread1)
   {
      delete gMasterThread1;
      gMasterThread1 = 0;
   }
   if (gMasterThread2)
   {
      delete gMasterThread2;
      gMasterThread2 = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
   return 0;
}
