/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h>

#define  _CMNPRIORITIES_CPP_
#include "cmnPriorities.h"

namespace Cmn
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

static const int tInt1 = THREAD_PRIORITY_TIME_CRITICAL;
static const int tInt2 = THREAD_PRIORITY_HIGHEST;
static const int tInt3 = THREAD_PRIORITY_ABOVE_NORMAL;
static const int tInt4 = THREAD_PRIORITY_NORMAL;
static const int tInt5 = THREAD_PRIORITY_BELOW_NORMAL;
static const int tInt6 = THREAD_PRIORITY_LOWEST;
static const int tInt7 = THREAD_PRIORITY_IDLE;


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Priorities::Priorities()
{
   mTsPrint           = Ris::Threads::Priority(-1, THREAD_PRIORITY_BELOW_NORMAL);
   mMasterLong        = Ris::Threads::Priority(-1, THREAD_PRIORITY_NORMAL);
   mMasterShort       = Ris::Threads::Priority(-1, THREAD_PRIORITY_HIGHEST);
   mSlave             = Ris::Threads::Priority(-1, THREAD_PRIORITY_ABOVE_NORMAL);
   mTimer             = Ris::Threads::Priority(-1, THREAD_PRIORITY_HIGHEST);
   mTest              = Ris::Threads::Priority(-1, THREAD_PRIORITY_ABOVE_NORMAL);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
