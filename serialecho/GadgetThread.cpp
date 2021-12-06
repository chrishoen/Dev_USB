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

#include "HidrawThread.h"

#define  _GADGETTHREAD_CPP_
#include "GadgetThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

GadgetThread::GadgetThread()
{
   // Set base class thread services.
   BaseClass::setThreadName("Gadget");
   BaseClass::setThreadPriorityHigh();

   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Initialize variables.
   mGadgetFd = -1;
   mEventFd = -1;
   mErrorCount = 0;
   mReportCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately
// after the thread starts running. It initializes something.

void GadgetThread::threadInitFunction()
{
   printf("GadgetThread::threadInitFunction\n");

   // Open the file.
   mGadgetFd = open(cGadgetDev, O_RDWR, S_IRUSR | S_IWUSR);
   if (mGadgetFd < 0)
   {
      perror("ERROR Gadget open");
   }

   // Open the shutdown event.
   mEventFd = eventfd(0, EFD_SEMAPHORE);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that waits for the
// hid keyboard input.

void GadgetThread::threadRunFunction()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Local variables.

   int tRet = 0;
   char tBuffer[256];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read report.

   while (!BaseClass::mTerminateFlag)
   {
      printf("Gadget read report********************************************* %d\n", mReportCount++);

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Read report.

      // Blocking poll for read or close.
      struct pollfd tPollFd[2];
      tPollFd[0].fd = mGadgetFd;
      tPollFd[0].events = POLLIN;
      tPollFd[0].revents = 0;
      tPollFd[1].fd = mEventFd;
      tPollFd[1].events = POLLIN;
      tPollFd[1].revents = 0;

      tRet = poll(&tPollFd[0], 2, -1);
      if (tRet < 0)
      {
         perror("ERROR Gadget poll");
         return;
      }

      // Test for close.
      if (tPollFd[1].revents & POLLIN)
      {
         printf("gadget read report closed\n");
         return;
      }

      // Not closed, read a report record.
      memset(tBuffer, 0x0, 32);
      tRet = read(mGadgetFd, tBuffer, 32);
      if (tRet < 0)
      {
         perror("ERROR Gadget read");
         return;
      }

      int tReportSize = tRet;
      printf("Gadget >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ");
      for (int i = 0; i < tRet; i++) printf("%hhx ", tBuffer[i]);
      printf("\n");

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Write report.

      // Copy the gadget file descriptor.
      if (gHidrawThread == 0) continue;
      int tHidrawFd = gHidrawThread->mHidrawFd;
      if (tHidrawFd <= 0) continue;

      tRet = write(tHidrawFd, tBuffer, 8);
      if (tRet < 0)
      {
         perror("ERROR Gadget write");
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated.

void GadgetThread::threadExitFunction()
{
   printf("GadgetThread::threadExitFunction\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This posts to the close event to
// terminate the thread and it closes the files.

void GadgetThread::shutdownThread()
{
   printf("GadgetThread::shutdownThread\n");

   // Request thread run function return.
   mTerminateFlag = true;

   // Write bytes to the event semaphore to signal a close. This will
   // cause the thread to terminate.
   unsigned long long tCount = 1;
   write(mEventFd, &tCount, 8);

   // Wait for the thread to terminate.
   BaseClass::waitForThreadTerminate();

   // Close the gadget file if it is open.
   if (mGadgetFd > 0)
   {
      close(mGadgetFd);
      mGadgetFd = -1;
   }

   // Close the event semaphore.
   close(mEventFd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
