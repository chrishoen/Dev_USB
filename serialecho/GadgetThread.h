#pragma once

/*==============================================================================
Kbd gadget thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsThreads.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a thread that processes the gadget inputs from the host.
// It owns the gadget file descriptor. It reads the gadget inputs from
// the host and writes them to the hidraw via the hidraw file descriptor
// that is owned by the hidraw thread.

class GadgetThread : public Ris::Threads::BaseThread
{
public:
   typedef Ris::Threads::BaseThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Device path for gadget.
   const char* cGadgetDev = "/dev/hidg0";

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // File descriptor for gadget.
   int mGadgetFd;

   // File descriptor for event semaphore used for close.
   int mEventFd;

   // Status.
   int mErrorCount;
   int mReportCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   GadgetThread();

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
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _GADGETTHREAD_CPP_
           GadgetThread* gGadgetThread = 0;
#else
   extern  GadgetThread* gGadgetThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************


