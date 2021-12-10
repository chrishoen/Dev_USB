/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <signal.h>
#include <poll.h>
#include <errno.h>
#include <sys/eventfd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/serial.h>

#define  _SLAVETHREAD_CPP_
#include "SlaveThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SlaveThread::SlaveThread()
{
   // Set base class thread services.
   BaseClass::setThreadName("Slave");
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

void SlaveThread::threadInitFunction()
{
   printf("SlaveThread::threadInitFunction\n");

   // Open the event.
   mEventFd = eventfd(0, EFD_SEMAPHORE);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that waits for the
// hid keyboard input.

void SlaveThread::threadRunFunction()
{
restart:
   // Guard.
   if (mTerminateFlag) return;

   // Sleep.
   BaseClass::threadSleep(1000);
   Prn::print(Prn::Show1, "Slave restart %d", mRestartCount++);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Local variables.

   int tRet = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open device.

   // If the device is open then close it.
   if (mPortFd > 0)
   {
      Prn::print(Prn::Show1, "Slave close");
      close(mPortFd);
      mPortFd = -1;
   }

   // Open the device.
   mPortFd = open(cPortDev, O_RDWR, S_IRUSR | S_IWUSR);
   if (mPortFd < 0)
   {
      Prn::print(Prn::Show1, "Slave open FAIL 101");
      goto restart;
   }

   // Configure the port for raw data.
   struct termios tOptions;
   tcgetattr(mPortFd, &tOptions);
   cfmakeraw(&tOptions);
   cfsetispeed(&tOptions, B115200);
   cfsetospeed(&tOptions, B115200);

   if (tcsetattr(mPortFd, TCSANOW, &tOptions) < 0)
   {
      Prn::print(Prn::Show1, "Slave open FAIL 102");
      goto restart;
   }

   Prn::print(Prn::Show1, "Slave open");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Loop to read string.

   while (!BaseClass::mTerminateFlag)
   {
      Prn::print(Prn::Show1, "Slave read start********************************************** %d", mRxCount++);

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Read string.

      // Blocking poll for read or abort.
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
         Prn::print(Prn::Show1, "Slave poll FAIL");
         goto restart;
      }

      // Test for abort.
      if (tPollFd[1].revents & POLLIN)
      {
         Prn::print(Prn::Show1, "Slave read abort");
         return;
      }

      // Read a request. 
      tRet = read(mPortFd, mRxBuffer, 200);
      if (tRet < 0)
      {
         Prn::print(Prn::Show1, "Slave read FAIL");
         goto restart;
      }
      if (tRet == 0)
      {
         Prn::print(Prn::Show1, "Slave read EMPTY");
         goto restart;
      }
      Prn::print(Prn::Show1, "Slave <<<<<<<<< %d", tRet);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It shuts down the hid api.

void SlaveThread::threadExitFunction()
{
   printf("SlaveThread::threadExitFunction\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This posts to the close event to
// terminate the thread and it closes the files.

void SlaveThread::shutdownThread()
{
   printf("SlaveThread::shutdownThread\n");

   // Request thread run function return.
   mTerminateFlag = true;

   // Write bytes to the event semaphore to signal a close. This will
   // cause the thread to terminate.
   unsigned long long tCount = 1;
   write(mEventFd, &tCount, 8);

   // Wait for the thread to terminate.
   BaseClass::waitForThreadTerminate();

   // Close the device if it is open.
   if (mPortFd > 0)
   {
      Prn::print(Prn::Show1, "Slave close");
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

void SlaveThread::sendString(const char* aString)
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
      Prn::print(Prn::Show1, "Slave write FAIL 101 %d", errno);
      return;
   }

   if (tRet != tNumBytes)
   {
      Prn::print(Prn::Show1, "Slave write FAIL 102");
      return;
   }

   Prn::print(Prn::Show1, "Slave write %d", tNumBytes);
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
