#pragma once

/*==============================================================================
Kbd definitions.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************
// These are keyboard modifier and keycode usage definitions.

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Modifier bits.
// hid1_11.pdf page 66 8.3 Report Format for Array Items.

static const char cKbdMod_LCtrl  = 0x01;
static const char cKbdMod_LShift = 0x02;
static const char cKbdMod_LAlt   = 0x04;
static const char cKbdMod_LGui   = 0x08;
static const char cKbdMod_RCtrl  = 0x10;
static const char cKbdMod_RShift = 0x20;
static const char cKbdMod_RAlt   = 0x40;
static const char cKbdMod_RGui   = 0x80;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Led bits.
// hid1_11.pdf page 70 B.1 Protocol 1 (Keyboard).

static const char cKbdLed_Caps = 0x02;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Key codes.
// hut1_12v2.pdf page 53 10 Keyboard/Keypad Page (0x07).

static const int cKbdCode_Caps = 0x39;
static const int cKbdCode_Space = 0x2c;

static const int cKbdCode_W = 0x1a;
static const int cKbdCode_E = 0x08;
static const int cKbdCode_R = 0x15;

static const int cKbdCode_Y = 0x1c;
static const int cKbdCode_U = 0x18;
static const int cKbdCode_I = 0x0c;
static const int cKbdCode_O = 0x12;
static const int cKbdCode_P = 0x13;

static const int cKbdCode_A = 0x04;
static const int cKbdCode_S = 0x16;
static const int cKbdCode_D = 0x07;

static const int cKbdCode_H = 0x0b;
static const int cKbdCode_J = 0x0d;
static const int cKbdCode_K = 0x0e;
static const int cKbdCode_L = 0x0f;

static const int cKbdCode_N = 0x11;
static const int cKbdCode_M = 0x10;
static const int cKbdCode_Comma = 0x36;
static const int cKbdCode_Period = 0x37;

static const int cKbdCode_Home = 0x4a;
static const int cKbdCode_LeftArrow = 0x50;
static const int cKbdCode_RightArrow = 0x4f;
static const int cKbdCode_End = 0x4d;
static const int cKbdCode_UpArrow = 0x52;
static const int cKbdCode_DownArrow = 0x51;
static const int cKbdCode_PageUp = 0x4b;
static const int cKbdCode_PageDown = 0x4e;

static const int cKbdCode_Esc = 0x29;
static const int cKbdCode_Delete = 0x4c;
static const int cKbdCode_Insert = 0x49;
static const int cKbdCode_Backspace = 0x2a;
static const int cKbdCode_Dash = 0x2d;

static const int cKbdCode_X = 0x1b;
static const int cKbdCode_C = 0x06;
static const int cKbdCode_V = 0x19;

//******************************************************************************
//******************************************************************************
//******************************************************************************


