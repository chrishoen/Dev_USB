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

#include "someUSBHostParms.h"

#define  _MASTERTHREAD_CPP_
#include "MasterThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

MasterThread::MasterThread(int aNum)
{
   // Instance variables.
   mNum = aNum;
   sprintf(mName, "Master%d", mNum);

   // Set base class variables.
   BaseClass::setThreadName(mName);
   BaseClass::setThreadPriorityHigh();

   // Set base class thread priority.
   BaseClass::setThreadPriorityHigh();

   // Initialize variables.
   if (mNum == 1)
   {
      strcpy(mPortPath, Some::gUSBHostParms.mHostDevPath1);
   }
   else
   {
      strcpy(mPortPath, Some::gUSBHostParms.mHostDevPath2);
   }

   mPortFd = -1;
   mEventFd = -1;
   mRxBuffer[0] = 0;
   mRxLength = 0;
   mBFlag = true;
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
   // Top of the loop.
   mRestartCount = 0;
restart:
   // Guard.
   if (mTerminateFlag) return;

   // Sleep.
   if (mRestartCount)
   {
      BaseClass::threadSleep(1000);
   }
   Prn::print(Prn::Show1, "%s restart %d %s", mName, mRestartCount, mPortPath);
   mRestartCount++;

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
      Prn::print(Prn::Show1, "%s close", mName);
      close(mPortFd);
      mPortFd = -1;
   }

   // Open the device.
   mPortFd = open(mPortPath, O_RDWR, S_IRUSR | S_IWUSR);
   if (mPortFd < 0)
   {
      Prn::print(Prn::Show1, "%s open FAIL 101", mName);
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
      Prn::print(Prn::Show1, "%s open FAIL 102", mName);
      goto restart;
   }

   Prn::print(Prn::Show1, "%s open", mName);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Loop to read from port.

   while (!BaseClass::mTerminateFlag)
   {
      Prn::print(Prn::Show4, "%s read start**********************************************", mName);

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Read from port.

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
         Prn::print(Prn::Show1, "%s poll FAIL", mName);
         goto restart;
      }

      // Test for abort.
      if (tPollFd[1].revents & POLLIN)
      {
         Prn::print(Prn::Show1, "%s read abort", mName);
         return;
      }

      // Read.
      tRet = read(mPortFd, mRxBuffer, cMaxBufferSize);
      if (tRet < 0)
      {
         Prn::print(Prn::Show1, "%s read FAIL", mName);
         goto restart;
      }
      if (tRet == 0)
      {
         Prn::print(Prn::Show1, "%s read EMPTY", mName);
         goto restart;
      }

      // No errors.
      mRxLength = tRet;

      // Null terminate.
      mRxBuffer[mRxLength] = 0;

      // Metrics.
      mRxCount++;

      // Print binary.
      if (mBFlag)
      {
         Prn::print(Prn::Show1, "%s read  IN  >>>>>>>>>> %2d %3d", mName,
            mRxCount, mRxLength);
      }
      // Print text.
      else
      {
         Prn::print(Prn::Show1, "%s read  IN  >>>>>>>>>> %2d %3d T %d %s", mName,
            mRxCount, mRxLength, my_trimCRLF(mRxBuffer), mRxBuffer);
      }
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

   // Close the device if it is open.
   if (mPortFd > 0)
   {
      Prn::print(Prn::Show1, "%s close", mName);
      close(mPortFd);
      mPortFd = -1;
   }

   // Close the event semaphore.
   close(mEventFd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a null terminated string via the usb port.

void MasterThread::sendString(const char* aString)
{
   // Guard.
   if (mPortFd < 0) return;

   // Metrics.
   mTxCount++;

   // Local variables.
   int tNumBytes = strlen(aString);
   int tRet = 0;

   // Write bytes to the port.
   tRet = write(mPortFd, aString, tNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      Prn::print(Prn::Show1, "%s write FAIL 101 %d", mName, errno);
      return;
   }
   if (tRet != tNumBytes)
   {
      Prn::print(Prn::Show1, "%s write FAIL 102", mName);
      return;
   }

   // Print.
   char tTxBuffer[100];
   strcpy(tTxBuffer, aString);
   Prn::print(Prn::Show1, "%s write OUT <<<<<<<<<< %2d %3d T %d %s", mName,
      mTxCount, tNumBytes, my_trimCRLF(tTxBuffer), tTxBuffer);
   
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send bytes via the usb port.

void MasterThread::sendBytes(const void* aBytes, int aNumBytes)
{
   // Guard.
   if (mPortFd < 0) return;

   // Metrics.
   mTxCount++;

   // Local variables.
   int tRet = 0;

   // Write bytes to the port.
   tRet = write(mPortFd, aBytes, aNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      Prn::print(Prn::Show1, "%s write FAIL 101 %d", mName, errno);
      return;
   }
   if (tRet != aNumBytes)
   {
      Prn::print(Prn::Show1, "%s write FAIL 102", mName);
      return;
   }

   // Print.
   Prn::print(Prn::Show1, "%s write OUT <<<<<<<<<< %2d %3d", mName,
      mTxCount, aNumBytes);

   return;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send test bytes via the usb port.

void MasterThread::sendTestBytes(int aNumBytes)
{
   // Allocate and fill some bytes.
   char* tBytes = new char[aNumBytes];
   for (int i = 0; i < aNumBytes; i++) tBytes[i] = 0x77;
   // Send the bytes.
   sendBytes(tBytes, aNumBytes);
   // Deallocate.
   delete tBytes;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
