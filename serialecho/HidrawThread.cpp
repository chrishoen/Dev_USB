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

#include "GadgetThread.h"
#include "Kbdtransform.h"

#define  _HIDRAWTHREAD_CPP_
#include "HidrawThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

HidrawThread::HidrawThread()
{
   // Set base class thread services.
   BaseClass::setThreadName("Hidraw");
   BaseClass::setThreadPriorityHigh();

   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Initialize variables.
   mHidrawFd = -1;
   mEventFd = -1;
   mErrorCount = 0;
   mReportCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately
// after the thread starts running. It initializes something.

void HidrawThread::threadInitFunction()
{
   printf("HidrawThread::threadInitFunction\n");

   // Open the event.
   mEventFd = eventfd(0, EFD_SEMAPHORE);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that waits for the
// hid keyboard input.

void HidrawThread::threadRunFunction()
{
restart:
   // Guard.
   if (mTerminateFlag) return;

   // Sleep.
   BaseClass::threadSleep(250);

   // If the hidraw file is open then close it.
   if (mHidrawFd > 0)
   {
      close(mHidrawFd);
      mHidrawFd = -1;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Local variables.

   int tRet = 0;
   int tReportDescSize = 0;
   char tBuffer[256];
   struct hidraw_report_descriptor tReportDesc;
   struct hidraw_devinfo tDevInfo;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open hidraw file.

   mHidrawFd = open(cHidrawDev, O_RDWR, S_IRUSR | S_IWUSR);

   if (mHidrawFd < 0)
   {
      perror("ERROR Hidraw open");
      goto restart;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read descriptors.

   // Get Report Descriptor Size.
   tRet = ioctl(mHidrawFd, HIDIOCGRDESCSIZE, &tReportDescSize);
   if (tRet < 0)
   {
      perror("ERROR Hidraw HIDIOCGRDESCSIZE");
      goto restart;
   }
   printf("Hidraw Report Descriptor Size: %d\n", tReportDescSize);

   // Get Report Descriptor.
   tReportDesc.size = tReportDescSize;
   tRet = ioctl(mHidrawFd, HIDIOCGRDESC, &tReportDesc);
   if (tRet < 0)
   {
      perror("ERROR Hidraw HIDIOCGRDESC");
      goto restart;
   }
   printf("Hidraw Report Descriptor:\n");
   for (int i = 0; i < tReportDesc.size; i++) printf("%hhx ", tReportDesc.value[i]);
   printf("\n");

   // Get Raw Name.
   tRet = ioctl(mHidrawFd, HIDIOCGRAWNAME(256), tBuffer);
   if (tRet < 0)
   {
      perror("ERROR Hidraw HIDIOCGRAWNAME");
      goto restart;
   }
   printf("Hidraw Raw Name: %s\n", tBuffer);

   // Get Physical Location. 
   tRet = ioctl(mHidrawFd, HIDIOCGRAWPHYS(256), tBuffer);
   if (tRet < 0)
   {
      perror("ERROR Hidraw HIDIOCGRAWPHYS");
      goto restart;
   }
   printf("Hidraw Raw Phys: %s\n", tBuffer);

   // Get Raw Info.
   tRet = ioctl(mHidrawFd, HIDIOCGRAWINFO, &tDevInfo);
   if (tRet < 0)
   {
      perror("ERROR Hidraw HIDIOCGRAWINFO");
      goto restart;
   }
   printf("Hidraw Raw Info:\n");
   printf("\tbustype: %d\n", tDevInfo.bustype);
   printf("\tvendor:  0x%04hx\n", tDevInfo.vendor);
   printf("\tproduct: 0x%04hx\n", tDevInfo.product);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read report.

   while (!BaseClass::mTerminateFlag)
   {
      printf("Hidraw read report********************************************** %d\n", mReportCount++);

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Read report.

      // IN report.
      char tReportA[32];

      // Blocking poll for read or close.
      struct pollfd tPollFd[2];
      tPollFd[0].fd = mHidrawFd;
      tPollFd[0].events = POLLIN;
      tPollFd[0].revents = 0;
      tPollFd[1].fd = mEventFd;
      tPollFd[1].events = POLLIN;
      tPollFd[1].revents = 0;

      tRet = poll(&tPollFd[0], 2, -1);
      if (tRet < 0)
      {
         perror("ERROR Hidraw poll");
         goto restart;
      }

      // Test for close.
      if (tPollFd[1].revents & POLLIN)
      {
         printf("Hidraw read report closed\n");
         goto restart;
      }

      // Not closed, read a report record. 
      tRet = read(mHidrawFd, tReportA, 32);
      if (tRet < 0)
      {
         perror("ERROR Hidraw read");
         goto restart;
      }
      printf("Hidraw <<<<<<<<< ");
      for (int i = 0; i < tRet; i++) printf("%hhx ", tReportA[i]);
      printf("\n");

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Write report.

      // Copy the gadget file descriptor.
      if (gGadgetThread == 0) continue;
      int tGadgetFd = gGadgetThread->mGadgetFd;
      if (tGadgetFd <= 0) continue;

      // Transform the read report to a write report.
      char tReportB[32];
      gKbdTransform.doTransformINReport(tReportA, tReportB);
      printf("Hidraw           ");
      for (int i = 0; i < tRet; i++) printf("%hhx ", tReportB[i]);
      printf("\n");

      // Write the transformed report to the host via the gadget.
      tRet = write(tGadgetFd, tReportB, 8);
      if (tRet < 0)
      {
         perror("ERROR Hidraw write");
      }
   }

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It shuts down the hid api.

void HidrawThread::threadExitFunction()
{
   printf("HidrawThread::threadExitFunction\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This posts to the close event to
// terminate the thread and it closes the files.

void HidrawThread::shutdownThread()
{
   printf("HidrawThread::shutdownThread\n");

   // Request thread run function return.
   mTerminateFlag = true;

   // Write bytes to the event semaphore to signal a close. This will
   // cause the thread to terminate.
   unsigned long long tCount = 1;
   write(mEventFd, &tCount, 8);

   // Wait for the thread to terminate.
   BaseClass::waitForThreadTerminate();

   // Close the hidraw file if it is open.
   if (mHidrawFd > 0)
   {
      close(mHidrawFd);
      mHidrawFd = -1;
   }

   // Close the event semaphore.
   close(mEventFd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
