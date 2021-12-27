//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"

#define  _SOMEUSBHOSTPARMS_CPP_
#include "someUSBHostParms.h"

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

USBHostParms::USBHostParms()
{
   reset();
}

void USBHostParms::reset()
{
   BaseClass::reset();
   if (Ris::portableIsWindows())
   {
      BaseClass::setFilePath("c:/aaa_prime/files/USBHost_Parms.txt");
   }
   else
   {
      BaseClass::setFilePath("/opt/prime/files/USBHost_Parms.txt");
   }

   mHostDevPath1[0] = 0;
   mHostDevPath2[0] = 0;
   mEnable1 = false;
   mEnable2 = false;
   mDelay1 = 0;
   mDelay2 = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void USBHostParms::show()
{
   printf("\n");
   printf("USBHostParms************************************************ %s\n", mTargetSection);

   printf("\n");
   printf("HostDevPath1             %-10s\n", mHostDevPath1);
   printf("HostDevPath2             %-10s\n", mHostDevPath2);
   printf("Enable1                  %-10s\n", my_string_from_bool(mEnable1));
   printf("Enable2                  %-10s\n", my_string_from_bool(mEnable2));
   printf("Delay1                   %-10d\n", mDelay1);
   printf("Delay2                   %-10d\n", mDelay2);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void USBHostParms::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("HostDevPath1"))          aCmd->copyArgString(1, mHostDevPath1, cMaxStringSize);
   if (aCmd->isCmd("HostDevPath2"))          aCmd->copyArgString(1, mHostDevPath2, cMaxStringSize);
   if (aCmd->isCmd("Enable1"))               mEnable1 = aCmd->argBool(1);
   if (aCmd->isCmd("Enable2"))               mEnable2 = aCmd->argBool(1);
   if (aCmd->isCmd("Delay1"))                mDelay1 = aCmd->argInt(1);
   if (aCmd->isCmd("Delay2"))                mDelay2 = aCmd->argInt(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void USBHostParms::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace