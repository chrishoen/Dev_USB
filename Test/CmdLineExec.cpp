#include "stdafx.h"

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
   if(aCmd->isCmd("RESET"  ))  reset();
   if(aCmd->isCmd("GO1"    ))  executeGo1(aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2(aCmd);
   if(aCmd->isCmd("GO3"    ))  executeGo3(aCmd);
   if(aCmd->isCmd("GO4"    ))  executeGo4(aCmd);
   if(aCmd->isCmd("GO5"    ))  executeGo5(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************

// Return a string from a host ordered address.
char* my_net_string_from_address(char* aBuffer, unsigned aAddress)
{
   union MyPacked
   {
      unsigned mUnsigned;
      unsigned char mBytes[4];
   };

   MyPacked tPacked;
   tPacked.mUnsigned = aAddress;

   sprintf(aBuffer, "%d.%d.%d.%d",
      (int)tPacked.mBytes[3],
      (int)tPacked.mBytes[2],
      (int)tPacked.mBytes[1],
      (int)tPacked.mBytes[0]);

   return aBuffer;
}

// Return true if a string represents a valid ip address.
bool my_net_is_ipaddress(const char* aIPAddress)
{
   int a0, a1, a2, a3 = 0;
   int tRet;
   tRet = sscanf(aIPAddress, "%d.%d.%d.%d", &a0, &a1, &a2, &a3);
   if (tRet != 4) return false;
   if (a0 < 0 || a0 > 255) return false;
   if (a1 < 0 || a1 > 255) return false;
   if (a2 < 0 || a2 > 255) return false;
   if (a3 < 0 || a3 > 255) return false;
   return true;
}

// Return a host ordered address from an ip address string.
unsigned my_net_address_from_string(char* aIPAddress)
{
   union MyPacked
   {
      unsigned mUnsigned;
      unsigned char mBytes[4];
   };

   MyPacked tPacked;
   tPacked.mUnsigned = 0xffffffff;

   int a0, a1, a2, a3 = 0;
   int tRet;
   tRet = sscanf(aIPAddress, "%d.%d.%d.%d", &a0, &a1, &a2, &a3);
   if (tRet != 4) return 0xfffffffe;
   if (a0 < 0 || a0 > 255) return 0xfffffffe;
   if (a1 < 0 || a1 > 255) return 0xfffffffe;
   if (a2 < 0 || a2 > 255) return 0xfffffffe;
   if (a3 < 0 || a3 > 255) return 0xfffffffe;

   tPacked.mBytes[0] = a3;
   tPacked.mBytes[1] = a2;
   tPacked.mBytes[2] = a1;
   tPacked.mBytes[3] = a0;

   return tPacked.mUnsigned;
}

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 0x00000000);
   unsigned tAddress = aCmd->argUnsigned(1);
   Prn::print(0, "Address %08x", tAddress);

   char tString[30];
   Prn::print(0, "Address %s", my_net_string_from_address(tString, tAddress));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0, "%d", my_net_is_ipaddress(aCmd->argString(1)));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0, "Address %08x", my_net_address_from_string(aCmd->argString(1)));
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

