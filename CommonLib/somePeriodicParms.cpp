//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"

#define  _SOMEPERIODICPARMS_CPP_
#include "somePeriodicParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

PeriodicParms::PeriodicParms()
{
   reset();
}

void PeriodicParms::reset()
{
   BaseClass::reset();
   if (Ris::portableIsWindows())
   {
      BaseClass::setFilePath("c:/aaa_prime/files/Periodic_Parms.txt");
   }
   else
   {
      BaseClass::setFilePath("/opt/prime/files/Periodic_Parms.txt");
   }

   mTestMode = 1;
   mTestCode1 = 0;
   mTestCode2 = 0;

   mMonitorThreadPeriod = 0;
   mTestThreadPeriod = 0;
   mTestThreadProcessor = -1;
   mTestThreadPriority = 80;
   mStatPeriod = 0;
   mSampleSize = 0;
   mPollProcessor = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void PeriodicParms::show()
{
   printf("\n");
   printf("PeriodicParms************************************************ %s\n", mTargetSection);

   printf("\n");
   printf("TestMode                 %-10d\n", mTestMode);
   printf("TestCode1                %-10d\n", mTestCode1);
   printf("TestCode2                %-10d\n", mTestCode2);
   printf("\n");
   printf("MonitorThreadPeriod      %-10d\n", mMonitorThreadPeriod);
   printf("TestThreadPeriod         %-10d\n", mTestThreadPeriod);
   printf("TestThreadProcessor      %-10d\n", mTestThreadProcessor);
   printf("TestThreadPriority       %-10d\n", mTestThreadPriority);
   printf("StatPeriod               %-10d\n", mStatPeriod);
   printf("SampleSize               %-10d\n", mSampleSize);
   printf("PollProcessor            %-10s\n", my_string_from_bool(mPollProcessor));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void PeriodicParms::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("TestMode"))              mTestMode = aCmd->argInt(1);
   if (aCmd->isCmd("TestCode1"))             mTestCode1 = aCmd->argInt(1);
   if (aCmd->isCmd("TestCode2"))             mTestCode2 = aCmd->argInt(1);

   if (aCmd->isCmd("MonitorThreadPeriod"))   mMonitorThreadPeriod = aCmd->argInt(1);
   if (aCmd->isCmd("TestThreadPeriod"))      mTestThreadPeriod = aCmd->argInt(1);
   if (aCmd->isCmd("TestThreadProcessor"))   mTestThreadProcessor = aCmd->argInt(1);
   if (aCmd->isCmd("TestThreadPriority"))    mTestThreadPriority = aCmd->argInt(1);
   if (aCmd->isCmd("StatPeriod"))            mStatPeriod = aCmd->argInt(1);
   if (aCmd->isCmd("SampleSize"))            mSampleSize = aCmd->argInt(1);
   if (aCmd->isCmd("PollProcessor"))         mPollProcessor = aCmd->argBool(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void PeriodicParms::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace