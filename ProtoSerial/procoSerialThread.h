#pragma once

/*==============================================================================
FCom serial thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsQCallThread.h"
#include "risSerialMsgThread.h"

#include "procoMsg.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Serial Thread.
//
// This is the serial thread class. It inherits from BaseQCallThread to
// obtain a call queue based thread functionality.
//
// The serial thread acts in conjunction with the serial message processor
// object. It passes received messages to the processor and gets back messages
// to transmit from the processor. The serial thread provides the execution
// context for the message processor to process the messages.
// 
// The serial thread has a member, mSerialMsgThread that is an instance of
// Ris::Net::SerialMsgThreadWithQCall. It is a child thread that manages
// connection session changes and receives messages as a Tcp serial. So, there
// are two threads structured as two layers: The serial thread and its member
// child thread mSerialMsgThread.
//
// The serial thread is based on a call queue and it uses QCalls to interface
// its mSerialMsgThread. When mSerialMsgThread detects a session change it
// invokes the serial thread's mSessionQCall, which defers execution of its
// executeSession member function. Likewise, when mSerialMsgThread receives
// a message it invokes the serial thread's mRxMsgQCall, which defers 
// execution of its executeRxMsg member function. 
//
// mSerialMsgThread provides the execution context for actually managing
// session changes and receiving messages. The session thread the provides
// the execution context for processing the session changes and the received 
// messages. The processing is done by the message processor object.
//

class  SerialThread : public Ris::Threads::BaseQCallThread
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Serial message thread, this manages serial message connections and
   // message transmission and reception.
   Ris::SerialMsgThread*  mSerialMsgThread;

   // Message monkey used by mSerialMsgThread.
   MsgMonkeyCreator mMonkeyCreator;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // State variables.
   bool mTPFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   typedef Ris::Threads::BaseQCallThread BaseClass;

   // Constructor.
   SerialThread();
  ~SerialThread();
  void show();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Thread base class overloads.

   // Thread init function. This is called by the base class immedidately 
   // after the thread starts running.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immedidately
   // before the thread is terminated.
   void threadExitFunction() override;

   // Execute periodically. This is called by the base class timer.
   void executeOnTimer(int aTimerCount) override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods: QCalls: These are used to send commands to the thread.

   // Example test qcall.

   // The qcall. This is a call that is queued to this thread.
   Ris::Threads::QCall1<int> mTest1QCall;

   // Execute the call in the context of the long thread.
   void executeTest1(
      int  aCode);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // QCalls registered to mSerialMsgThread
   Ris::SerialMsgThread::RxMsgQCall    mRxMsgQCall;

   // Associated QCall methods, these are called by the
   // threadRunFunction to process conditions sent from 
   // mTcpServerThread.
   void executeRxMsg (Ris::ByteContent* aRxMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Receive message handlers.
   void processRxMsg(ProtoComm::TestMsg*  aMsg);
   void processRxMsg(ProtoComm::EchoRequestMsg* aMsg);
   void processRxMsg(ProtoComm::EchoResponseMsg* aMsg);
   void processRxMsg(ProtoComm::DataMsg* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a message via mSerialMsgThread:
   void sendMsg (BaseMsg* aTxMsg);
   void sendTestMsg();
   void sendSettingsMsg();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _PROCOSERIALTHREAD_CPP_
         SerialThread* gSerialThread;
#else
extern   SerialThread* gSerialThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
