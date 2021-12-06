
#include "stdafx.h"

#include "risThreadsProcess.h"

#include "procoSerialSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize.

void main_initialize(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Enter process.

   Ris::Threads::enterProcessHigh();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize print facility.

   Prn::resetPrint();
   Prn::useConsole(1);
   Prn::initializePrint();

   // Initialize print filters.
   Prn::setFilter(Prn::SerialInitP1,    true);
   Prn::setFilter(Prn::SerialInitP2,    true);
   Prn::setFilter(Prn::SerialRxRunP1,   false);
   Prn::setFilter(Prn::SerialRxRunP2,   false);
   Prn::setFilter(Prn::SerialTxRunP1,   false);
   Prn::setFilter(Prn::SerialTxRunP2,   false);
   Prn::setFilter(Prn::SerialErrorP1,   true);
   Prn::setFilter(Prn::SerialErrorP2,   true);

   Prn::setFilter(Prn::SerialInitM1,    true);
   Prn::setFilter(Prn::SerialInitM2,    true);
   Prn::setFilter(Prn::SerialRxRunM1,   false);
   Prn::setFilter(Prn::SerialRxRunM2,   false);
   Prn::setFilter(Prn::SerialTxRunM1,   false);
   Prn::setFilter(Prn::SerialTxRunM2,   false);
   Prn::setFilter(Prn::SerialErrorM1,   true);
   Prn::setFilter(Prn::SerialErrorM2,   true);

   Prn::setFilter(Prn::ThreadInit1,     true);
   Prn::setFilter(Prn::ThreadInit1,     true);
   Prn::setFilter(Prn::ThreadRun1,      true);
   Prn::setFilter(Prn::ThreadRun2,      true);
   Prn::setFilter(Prn::ThreadRun3,      true);
   Prn::setFilter(Prn::ThreadRun4,      true);

   Prn::setFilter(Prn::ProcRun1,        true);
   Prn::setFilter(Prn::ProcRun2,        true);
   Prn::setFilter(Prn::ProcRun3,        false);
   Prn::setFilter(Prn::ProcRun4,        true);

   Prn::setFilter(Prn::ViewRun1,        true,  1);
   Prn::setFilter(Prn::ViewRun2,        true,  1);
   Prn::setFilter(Prn::ViewRun3,        false, 1);
   Prn::setFilter(Prn::ViewRun4,        true,  1);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read parameters files.

   ProtoComm::gSerialSettings.reset();
   ProtoComm::gSerialSettings.readSection("default");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   Prn::print(0,"ProtoCommS Program**********************************************BEGIN");

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize.

void main_finalize()
{
   Prn::print(0,"ProtoCommS Program**********************************************END");

   // Close print
   Prn::finalizePrint();

   // Exit process
   Ris::Threads::exitProcess();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
