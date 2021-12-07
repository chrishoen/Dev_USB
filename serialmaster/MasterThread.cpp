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

#define  _MASTERTHREAD_CPP_
#include "MasterThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

MasterThread::MasterThread()
{
   // Set base class thread services.
   BaseClass::setThreadName("Master");
   BaseClass::setThreadPriorityHigh();

   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Initialize variables.
   mPortFd = -1;
   mEventFd = -1;
   mRxBuffer[0] = 0;
   mErrorCount = 0;
   mRestartCount = 0;
   mRxCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately
// after the thread starts running. It initializes something.

void MasterThread::threadInitFunction()
{
   printf("MasterThread::threadInitFunction\n");

   // Open the event.
   mEventFd = eventfd(0, EFD_SEMAPHORE);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that waits for the
// hid keyboard input.

void MasterThread::threadRunFunction()
{
restart:
   // Guard.
   if (mTerminateFlag) return;

   // Sleep.
   BaseClass::threadSleep(1000);
   Prn::print(Prn::View21, "Master restart %d", mRestartCount++);

   // If the hidraw file is open then close it.
   if (mPortFd > 0)
   {
      Prn::print(Prn::View21, "Master close");
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

   Prn::print(Prn::View21, "Master open");
   mPortFd = open(cPortDev, O_RDWR, S_IRUSR | S_IWUSR);
   if (mPortFd < 0)
   {
      Prn::print(Prn::View21, "Master open FAIL");
      goto restart;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read report.

   while (!BaseClass::mTerminateFlag)
   {
      Prn::print(Prn::View21, "Master read start********************************************** %d", mRxCount++);

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
         Prn::print(Prn::View21, "Master poll FAIL");
         goto restart;
      }

      // Test for abort.
      if (tPollFd[1].revents & POLLIN)
      {
         Prn::print(Prn::View21, "Master read abort");
         return;
      }

      // Not abort, read a request. 
      tRet = read(mPortFd, mRxBuffer, 32);
      if (tRet < 0)
      {
         Prn::print(Prn::View21, "Master read FAIL");
         goto restart;
      }
      if (tRet == 0)
      {
         Prn::print(Prn::View21, "Master read EMPTY");
         goto restart;
      }
      Prn::print(Prn::View21, "Master <<<<<<<<< ");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It shuts down the hid api.

void MasterThread::threadExitFunction()
{
   printf("MasterThread::threadExitFunction\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This posts to the close event to
// terminate the thread and it closes the files.

void MasterThread::shutdownThread()
{
   printf("MasterThread::shutdownThread\n");

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
      Prn::print(Prn::View21, "Master close");
      close(mPortFd);
      mPortFd = -1;
   }

   // Close the event semaphore.
   close(mEventFd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a null terminated string via the serial port. A newline terminator
// is appended to the string before transmission. This executes in the
// context of the calling thread.

void MasterThread::sendString(const char* aString)
{
   // Guard.
   if (mPortFd < 0) return;

   // Local variables.
   int tNumBytes = strlen(aString);
   int tRet = 0;

   // Write bytes to the port.
   tRet = write(mPortFd, aString, tNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      Prn::print(Prn::View21, "Master write FAIL 101 %d", errno);
      return;
   }

   if (tRet != tNumBytes)
   {
      Prn::print(Prn::View21, "Master write FAIL 102");
      return;
   }

   Prn::print(Prn::View21, "Master write %d", tNumBytes);
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
