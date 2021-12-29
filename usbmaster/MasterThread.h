#pragma once

/*==============================================================================
Kbd hidraw thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsThreads.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a thread that processes the hidraw inputs from the keyboard.
// It owns the hidraw file descriptor. It reads the hidraw inputs from
// the keyboard, modally translates them, and writes them to the host 
// via the gadget file descriptor that is owned by the gadget thread.

class MasterThread : public Ris::Threads::BaseThread
{
public:
   typedef Ris::Threads::BaseThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Sizes.
   static const int cMaxBufferSize = 4096;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Thread instance number.
   int mNum;

   // Thread instance name.
   char mName[32];

   // File path for usb port.
   char mPortPath[64];

   // File descriptor for usb port.
   int mPortFd;

   // File descriptor for event semaphore used for close.
   int mEventFd;

   // Receive buffer.
   char mRxBuffer[cMaxBufferSize];

   // Receive buffer length.
   int mRxLength;

   // If true then show in binary mode.
   bool mBFlag;

   // Metrics.
   int mErrorCount;
   int mRestartCount;
   int mRxCount;
   int mTxCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   MasterThread(int aNum);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Base class overloads.

   // Thread init function. This is called by the base class immediately
   // after the thread starts running. It initializes something.
   void threadInitFunction() override;

   // Thread run function. This is called by the base class immediately
   // after the thread init function. It runs a loop that waits for the
   // hid keyboard input.
   void threadRunFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated. It is a placeholder.
   void threadExitFunction() override;

   // Thread shutdown function. This posts to the close event to
   // terminate the thread and it closes the files.
   void shutdownThread() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a null terminated string via the usb port.
   void sendString(const char* aString);

   // Send bytes via the usb port.
   void sendBytes(const void* aBytes, int aNumBytes);

   // Send test bytes via the usb port.
   void sendTestBytes(int aNumBytes);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _MASTERTHREAD_CPP_
MasterThread* gMasterThread1 = 0;
MasterThread* gMasterThread2 = 0;
#else
extern  MasterThread* gMasterThread1;
extern  MasterThread* gMasterThread2;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************


