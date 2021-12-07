/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/eventfd.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>

#define  _ECHOTHREAD_CPP_
#include "EchoThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

EchoThread::EchoThread()
{
   // Set base class thread services.
   BaseClass::setThreadName("Echo");
   BaseClass::setThreadPriorityHigh();

   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Initialize variables.
   mPortFd = -1;
   mEventFd = -1;
   mRequest[0] = 0;
   mErrorCount = 0;
   mRestartCount = 0;
   mRequestCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately
// after the thread starts running. It initializes something.

void EchoThread::threadInitFunction()
{
   printf("EchoThread::threadInitFunction\n");

   // Open the event.
   mEventFd = eventfd(0, EFD_SEMAPHORE);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that waits for the
// hid keyboard input.

void EchoThread::threadRunFunction()
{
restart:
   // Guard.
   if (mTerminateFlag) return;

   // Sleep.
   BaseClass::threadSleep(1000);
   Prn::print(Prn::View11, "Echo restart %d", mRestartCount++);

   // If the hidraw file is open then close it.
   if (mPortFd > 0)
   {
      close(mPortFd);
      mPortFd = -1;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Local variables.

   int tRet = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open hidraw file.

   Prn::print(Prn::View11, "Echo open");
   mPortFd = open(cPortDev, O_RDWR, S_IRUSR | S_IWUSR);

   if (mPortFd < 0)
   {
      Prn::print(Prn::View11, "Echo open FAIL");
      goto restart;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read report.

   while (!BaseClass::mTerminateFlag)
   {
      Prn::print(Prn::View11, "Echo read request********************************************** %d\n", mRequestCount++);

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Read report.

      // Blocking poll for read or close.
      struct pollfd tPollFd[2];
      tPollFd[0].fd = mPortFd;
      tPollFd[0].events = POLLIN;
      tPollFd[0].revents = 0;
      tPollFd[1].fd = mEventFd;
      tPollFd[1].events = POLLIN;
      tPollFd[1].revents = 0;

      tRet = poll(&tPollFd[0], 2, -1);
      if (tRet < 0)
      {
         Prn::print(Prn::View11, "Echo poll FAIL");
         goto restart;
      }

      // Test for abort.
      if (tPollFd[1].revents & POLLIN)
      {
         Prn::print(Prn::View11, "Echo read abort\n");
         return;
      }

      // Not closed, read a request. 
      tRet = read(mPortFd, mRequest, 32);
      if (tRet < 0)
      {
         Prn::print(Prn::View11, "Echo read FAIL");
         goto restart;
      }
      if (tRet == 0)
      {
         Prn::print(Prn::View11, "Echo read EMPTY");
         goto restart;
      }
      Prn::print(Prn::View11, "Echo <<<<<<<<< ");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It shuts down the hid api.

void EchoThread::threadExitFunction()
{
   printf("EchoThread::threadExitFunction\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This posts to the close event to
// terminate the thread and it closes the files.

void EchoThread::shutdownThread()
{
   printf("EchoThread::shutdownThread\n");

   // Request thread run function return.
   mTerminateFlag = true;

   // Write bytes to the event semaphore to signal a close. This will
   // cause the thread to terminate.
   unsigned long long tCount = 1;
   write(mEventFd, &tCount, 8);

   // Wait for the thread to terminate.
   BaseClass::waitForThreadTerminate();

   // Close the hidraw file if it is open.
   if (mPortFd > 0)
   {
      close(mPortFd);
      mPortFd = -1;
   }

   // Close the event semaphore.
   close(mEventFd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
