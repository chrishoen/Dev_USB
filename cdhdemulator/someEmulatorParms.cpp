//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"

#define  _SOMEEMULATORPARMS_CPP_
#include "someEmulatorParms.h"

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

EmulatorParms::EmulatorParms()
{
   reset();
}

void EmulatorParms::reset()
{
   BaseClass::reset();
   if (Ris::portableIsWindows())
   {
      BaseClass::setFilePath("c:/aaa_prime/files/Emulator_Parms.txt");
   }
   else
   {
      BaseClass::setFilePath("/opt/prime/files/Emulator_Parms.txt");
   }

   mEmulatorDevPath[0] = 0;

   // Delays.
   mDelay1 = 0;
   mDelay2 - 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void EmulatorParms::show()
{
   printf("\n");
   printf("EmulatorParms************************************************ %s\n", mTargetSection);

   printf("\n");
   printf("EmulatorDevPath          %-10s\n", mEmulatorDevPath);
   printf("Delay1                   %-10d\n", mDelay1);
   printf("Delay2                   %-10d\n", mDelay2);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void EmulatorParms::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("EmulatorDevPath"))       aCmd->copyArgString(1, mEmulatorDevPath, cMaxStringSize);
   if (aCmd->isCmd("Delay1"))                mDelay1 = aCmd->argInt(1);
   if (aCmd->isCmd("Delay2"))                mDelay2 = aCmd->argInt(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void EmulatorParms::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace