/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "procoMsgHelper.h"
#include "procoSerialSettings.h"

#define  _PROCOSERIALTHREAD_CPP_
#include "procoSerialThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialThread::SerialThread()
{
   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Set timer period.
   BaseClass::mTimerPeriod = 1000;

   // Initialize QCalls.
   mRxMsgQCall.bind   (this,&SerialThread::executeRxMsg);
   mTest1QCall.bind   (this,&SerialThread::executeTest1);

   // Initialize variables.
   mSerialMsgThread = 0;
   mTPFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialThread::~SerialThread()
{
   if (mSerialMsgThread) delete mSerialMsgThread;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialThread::show()
{
   Prn::print(0, "SerialMsgPort %d %d",
      mSerialMsgThread->mSerialMsgPort.mHeaderAllCount,
      mSerialMsgThread->mSerialMsgPort.mHeaderOneCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SerialThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "SerialThread::threadInitFunction");

   // Instance of serial port settings.
   Ris::SerialSettings tSerialSettings;

   tSerialSettings.setPortDevice(gSerialSettings.mSerialPortDevice);
   tSerialSettings.setPortSetup(gSerialSettings.mSerialPortSetup);
   tSerialSettings.mRxTimeout     = gSerialSettings.mSerialRxTimeout;
   tSerialSettings.mMonkeyCreator = &mMonkeyCreator;
   tSerialSettings.mRxMsgQCall    = mRxMsgQCall;

   // Create child thread.
   mSerialMsgThread = new Ris::SerialMsgThread(tSerialSettings);

   // Launch child thread.
   mSerialMsgThread->launchThread(); 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void  SerialThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "SerialThread::threadExitFunction");

   // Shutdown the tcp client thread
   mSerialMsgThread->shutdownThread();

   // Base class exit
   BaseClass::threadExitFunction();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute the call in the context of the long thread.

void SerialThread::executeTest1(int  aCode)
{
   TestMsg* msg = new TestMsg;
   msg->mCode1 = 201;

   mSerialMsgThread->sendMsg(msg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// executeOnTimer

void SerialThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;

   EchoRequestMsg* tMsg = new EchoRequestMsg;
   tMsg->mCode1 = aTimerCount;
   sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall

void SerialThread::executeRxMsg(Ris::ByteContent* aMsg)
{
   ProtoComm::BaseMsg* tMsg = (ProtoComm::BaseMsg*)aMsg;

   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (tMsg->mMessageType)
   {
   case ProtoComm::MsgIdT::cTestMsg:
      processRxMsg((ProtoComm::TestMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cEchoRequestMsg:
      processRxMsg((ProtoComm::EchoRequestMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cEchoResponseMsg:
      processRxMsg((ProtoComm::EchoResponseMsg*)tMsg);
      break;
   case ProtoComm::MsgIdT::cDataMsg:
      processRxMsg((ProtoComm::DataMsg*)tMsg);
      break;
   default:
      Prn::print(Prn::ThreadRun1, "SerialThread::executeServerRxMsg ??? %d", tMsg->mMessageType);
      delete tMsg;
      break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Message handler for TestMsg.

void SerialThread::processRxMsg(ProtoComm::TestMsg*  aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoRequestMsg

void SerialThread::processRxMsg(ProtoComm::EchoRequestMsg* aMsg)
{
   if (true)
   {
      ProtoComm::EchoResponseMsg* tMsg = new ProtoComm::EchoResponseMsg;
      mSerialMsgThread->sendMsg(tMsg);
   }

   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - EchoResponseMsg

void SerialThread::processRxMsg(ProtoComm::EchoResponseMsg* aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rx message handler - DataMsg

void SerialThread::processRxMsg(ProtoComm::DataMsg* aMsg)
{
   MsgHelper::show(aMsg);
   delete aMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Sends a message via the serial thread.

void SerialThread::sendMsg(BaseMsg* aTxMsg)
{
   mSerialMsgThread->sendMsg(aTxMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a message via the serial thread.

void SerialThread::sendTestMsg()
{
   TestMsg* tMsg = new TestMsg;
   tMsg->mCode1 = 201;

   mSerialMsgThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace