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

class SlaveThread : public Ris::Threads::BaseThread
{
public:
   typedef Ris::Threads::BaseThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Device path for usb acm.
   const char* cPortDev = "/dev/ttyGS0";

   // Device path for usb acm.
   static const int cMaxStringSize = 256;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // File descriptor for usb acm.
   int mPortFd;

   // File descriptor for event semaphore used for close.
   int mEventFd;

   // Request buffer.
   char mRxBuffer[cMaxStringSize];

   // Status.
   int mErrorCount;
   int mRestartCount;
   int mRxCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SlaveThread();

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

   // Send a null terminated string via the serial port. A newline terminator
   // is appended to the string before transmission. This executes in the
   // context of the calling thread.
   void sendString(const char* aString);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _SLAVETHREAD_CPP_
           SlaveThread* gSlaveThread = 0;
#else
   extern  SlaveThread* gSlaveThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************


