#pragma once

/*==============================================================================
Kbd modal transformation.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class provides a modal keyboard transformation for key codes
// and modifier bits. It supplies a function that operates on hid keyboard
// reports (ReportA->ReportB).
//
// If the caps lock key is not pressed then the mode is normal and
// an identify transformation is applied, all key codes and modifier
// bits are returned with no changes.
//
// If the caps lock key is pressed then the mode is special and
// a different transformation is applied, some key codes and modifier
// bits are returned with changes and some are ignored.
// 
// An example of this is pressing caps lock and 'u' produces an
// up arrow and caps lock and 'm' produces a down arrow.
// 

class KbdTransform
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If false then the mode is normal. If true then the mode is special.
   // The mode is special when the caps lock key is pressed.
   bool mSpecMode;

   // If true then set the ReportB modifier control, alt, shift bits.
   bool mSpecCtrl;
   bool mSpecAlt;
   bool mSpecGui;
   bool mSpecShift;

   // Intermediate temporary report.
   char mSpecReport[16];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   KbdTransform();
   void reset();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Transform an IN report. An IN report is an input to the host.
   // Apply a function, ReportA->ReportB, that depends on the mode.
   // The mode is changed by the caps lock key. 
   void doTransformINReport(const char* aReportA, char* aReportB);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Examine SpecReport to determine the mode. If all of the bytes are
   // zero then set the mode to normal. If one of the keycodes is capslock
   // then set the mode to special. Reset the modifier flags.
   void doProcessINForSpecial();

   // Transform a SpecReport keycode and set modifier flags.
   void doTransformINReportKey(int aKeyIndex);

   // Remove SpecReport zero key codes.
   void doRemoveINReportZeroes();

   // OR the modifier flags into the SpecReport modifier byte.
   void doTransformINReportModifier();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _KBDTRANSFORM_CPP_
           KbdTransform gKbdTransform;
#else
   extern  KbdTransform gKbdTransform;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************


