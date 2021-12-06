/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "KbdDefs.h"

#define  _KBDTRANSFORM_CPP_
#include "KbdTransform.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

KbdTransform::KbdTransform()
{
   reset();
}

void KbdTransform::reset()
{
   mSpecMode = false;
   mSpecCtrl = false;
   mSpecAlt = false;
   mSpecGui = false;
   mSpecShift = false;
   memset(mSpecReport, 0, 8);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Transform an IN report. An IN report is an input to the host.
// Apply a function, ReportA->ReportB, that depends on the mode.
// The mode is changed by the caps lock key. 

void KbdTransform::doTransformINReport(const char* aReportA, char* aReportB)
{
   // Copy ReportA to SpecReport.
   memcpy(mSpecReport, aReportA, 8);

   // Examine SpecReport to determine the mode.
   doProcessINForSpecial();

   // If special mode then, transform the keycodes, remove zeros, 
   // and set the modifier bits.
   if (mSpecMode)
   {
      // Transform SpecReport keycodes and set modifier flags.
      for (int i = 2; i < 8; i++)
      {
         doTransformINReportKey(i);
      }

      // Remove zero keycodes from SpecReport.
      doRemoveINReportZeroes();

      // Set SpecReport modifier bits.
      doTransformINReportModifier();
   }

   // Copy SpecReport to ReportB.
   memcpy(aReportB, mSpecReport, 8);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Examine SpecReport to determine the mode. If all of the bytes are
// zero then set the mode to normal. If one of the keycodes is capslock
// then set the mode to special. Reset the modifier flags.

void KbdTransform::doProcessINForSpecial()
{
   // Check for all zeroes.
   bool tAllZero = true;
   for (int i = 0; i < 8; i++)
   {
      if (mSpecReport[i]) tAllZero = false;
   }

   // If all zeroes then set the mode to normal.
   if (tAllZero)
   {
      mSpecMode = false;
   }

   // Check the keycodes for capslock. If there is a capslock then
   // set the mode to special and zero the keycode to ignore it.
   for (int i = 2; i < 8; i++)
   {
      if (mSpecReport[i] == cKbdCode_Caps)
      {
         mSpecMode = true;
         mSpecReport[i] = 0;
      }
   }

   // Reset the modifier flags.
   mSpecCtrl = false;
   mSpecAlt = false;
   mSpecGui = false;
   mSpecShift = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Transform a SpecReport keycode and set modifier flags.

void KbdTransform::doTransformINReportKey(int aKeyIndex)
{
   char tKeyA = mSpecReport[aKeyIndex];
   char tKeyB = 0;

   // KeyA->KeyB.
   switch (tKeyA)
   {
   case cKbdCode_A: tKeyB = cKbdCode_LeftArrow; mSpecGui = true; mSpecCtrl = true; break;
   case cKbdCode_S: tKeyB = cKbdCode_RightArrow; mSpecGui = true; mSpecCtrl = true; break;
   case cKbdCode_X: tKeyB = cKbdCode_Caps; break;

   case cKbdCode_D: mSpecCtrl = true; break;
   case cKbdCode_Space: mSpecShift = true; break;

   case cKbdCode_W: tKeyB = cKbdCode_X; mSpecCtrl = true; break;
   case cKbdCode_E: tKeyB = cKbdCode_C; mSpecCtrl = true; break;
   case cKbdCode_R: tKeyB = cKbdCode_V; mSpecCtrl = true; break;

   case cKbdCode_Y: tKeyB = cKbdCode_Esc; break;
   case cKbdCode_U: tKeyB = cKbdCode_UpArrow; break;
   case cKbdCode_I: tKeyB = cKbdCode_Delete; break;
   case cKbdCode_O: tKeyB = cKbdCode_Insert; break;
   case cKbdCode_P: tKeyB = cKbdCode_Dash; break;

   case cKbdCode_H: tKeyB = cKbdCode_Home; break;
   case cKbdCode_J: tKeyB = cKbdCode_LeftArrow; break;
   case cKbdCode_K: tKeyB = cKbdCode_RightArrow; break;
   case cKbdCode_L: tKeyB = cKbdCode_End; break;

   case cKbdCode_N: tKeyB = cKbdCode_Backspace; break;
   case cKbdCode_M: tKeyB = cKbdCode_DownArrow; break;
   case cKbdCode_Comma: tKeyB = cKbdCode_PageUp; break;
   case cKbdCode_Period: tKeyB = cKbdCode_PageDown; break;
   }

   // Store KeyB.
   mSpecReport[aKeyIndex] = tKeyB;
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Remove SpecReport zero key codes.

void KbdTransform::doRemoveINReportZeroes()
{
   // Copy SpecReport to temp report.
   char tReport[16];
   memcpy(tReport, mSpecReport, 8);

   // Set SpecReport keycodes to zero.
   for (int i = 2; i < 8; i++)
   {
      mSpecReport[i] = 0;
   }

   // Copy temp report nonzero keycodes to SpecReport.
   int j = 2;
   for (int i = 2; i < 8; i++)
   {
      if (tReport[i])
      {
         mSpecReport[j++] = tReport[i];
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// OR the modifier flags into the SpecReport modifier byte.

void KbdTransform::doTransformINReportModifier()
{
   char tModifierB = mSpecReport[0];

   if (mSpecAlt) tModifierB |= cKbdMod_LAlt;
   if (mSpecCtrl) tModifierB |= cKbdMod_LCtrl;
   if (mSpecGui) tModifierB |= cKbdMod_LGui;
   if (mSpecShift) tModifierB |= cKbdMod_LShift;

   mSpecReport[0] = tModifierB;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
