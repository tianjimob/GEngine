#pragma once

#include "function/framework/input/input_types.h"
#include <cstdint>

#define SCANCODE_MASK (1 << 30)
#define SCANCODE_TO_KEYCODE(X) (static_cast<uint32_t>(X) | SCANCODE_MASK)

namespace GEngine {

enum class ModifierKey {
  None,

  LeftShift,
  RightShift,
  LeftCtrl,
  RightCtrl,
  LeftAlt,
  RightAlt,
  LeftGui, // often windows key
  RightGui,
  NumLock, // maybe located on an extented keypad
  CapsLock,

  Shift, // LeftShift and RightShift
  Ctrl,  // LeftCtrl and RightCtrl
  Alt,   // LeftAlt and RightAlt
  Gui,   // LeftGui and RightGui
};

enum class ScanCode : uint32_t {
  SCANCODE_UNKNOWN = 0,

  /**
   *  \name Usage page 0x07
   *
   *  These values are from usage page 0x07 (USB keyboard page).
   */
  /* @{ */

  A = 4,
  B = 5,
  C = 6,
  D = 7,
  E = 8,
  F = 9,
  G = 10,
  H = 11,
  I = 12,
  J = 13,
  K = 14,
  L = 15,
  M = 16,
  N = 17,
  O = 18,
  P = 19,
  Q = 20,
  R = 21,
  S = 22,
  T = 23,
  U = 24,
  V = 25,
  W = 26,
  X = 27,
  Y = 28,
  Z = 29,

  NUM_1 = 30,
  NUM_2 = 31,
  NUM_3 = 32,
  NUM_4 = 33,
  NUM_5 = 34,
  NUM_6 = 35,
  NUM_7 = 36,
  NUM_8 = 37,
  NUM_9 = 38,
  NUM_0 = 39,

  Return = 40,
  Escape = 41,
  Backspace = 42,
  Tab = 43,
  Space = 44,

  MINUS = 45,
  EQUALS = 46,
  LEFTBRACKET = 47,
  RIGHTBRACKET = 48,
  BACKSLASH = 49, /**< Located at the lower left of the return
                   *   key on ISO keyboards and at the right end
                   *   of the QWERTY row on ANSI keyboards.
                   *   Produces REVERSE SOLIDUS (backslash) and
                   *   VERTICAL LINE in a US layout, REVERSE
                   *   SOLIDUS and VERTICAL LINE in a UK Mac
                   *   layout, NUMBER SIGN and TILDE in a UK
                   *   Windows layout, DOLLAR SIGN and POUND SIGN
                   *   in a Swiss German layout, NUMBER SIGN and
                   *   APOSTROPHE in a German layout, GRAVE
                   *   ACCENT and POUND SIGN in a French Mac
                   *   layout, and ASTERISK and MICRO SIGN in a
                   *   French Windows layout.
                   */
  NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                   *   instead of 49 for the same key, but all
                   *   OSes I've seen treat the two codes
                   *   identically. So, as an implementor, unless
                   *   your keyboard generates both of those
                   *   codes and your OS treats them differently,
                   *   you should generate BACKSLASH
                   *   instead of this code. As a user, you
                   *   should not rely on this code because SDL
                   *   will never generate it with most (all?)
                   *   keyboards.
                   */
  SEMICOLON = 51,
  APOSTROPHE = 52,
  GRAVE = 53, /**< Located in the top left corner (on both ANSI
               *   and ISO keyboards). Produces GRAVE ACCENT and
               *   TILDE in a US Windows layout and in US and UK
               *   Mac layouts on ANSI keyboards, GRAVE ACCENT
               *   and NOT SIGN in a UK Windows layout, SECTION
               *   SIGN and PLUS-MINUS SIGN in US and UK Mac
               *   layouts on ISO keyboards, SECTION SIGN and
               *   DEGREE SIGN in a Swiss German layout (Mac:
               *   only on ISO keyboards), CIRCUMFLEX ACCENT and
               *   DEGREE SIGN in a German layout (Mac: only on
               *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
               *   French Windows layout, COMMERCIAL AT and
               *   NUMBER SIGN in a French Mac layout on ISO
               *   keyboards, and LESS-THAN SIGN and GREATER-THAN
               *   SIGN in a Swiss German, German, or French Mac
               *   layout on ANSI keyboards.
               */
  COMMA = 54,
  PERIOD = 55,
  SLASH = 56,

  CAPSLOCK = 57,

  F1 = 58,
  F2 = 59,
  F3 = 60,
  F4 = 61,
  F5 = 62,
  F6 = 63,
  F7 = 64,
  F8 = 65,
  F9 = 66,
  F10 = 67,
  F11 = 68,
  F12 = 69,

  PRINTSCREEN = 70,
  SCROLLLOCK = 71,
  PAUSE = 72,
  INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                 does send code 73, not 117) */
  HOME = 74,
  PAGEUP = 75,
  DEL = 76,
  END = 77,
  PAGEDOWN = 78,
  RIGHT = 79,
  LEFT = 80,
  DOWN = 81,
  UP = 82,

  NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                      */
  KP_DIVIDE = 84,
  KP_MULTIPLY = 85,
  KP_MINUS = 86,
  KP_PLUS = 87,
  KP_ENTER = 88,
  KP_1 = 89,
  KP_2 = 90,
  KP_3 = 91,
  KP_4 = 92,
  KP_5 = 93,
  KP_6 = 94,
  KP_7 = 95,
  KP_8 = 96,
  KP_9 = 97,
  KP_0 = 98,
  KP_PERIOD = 99,

  NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                         *   keyboards have over ANSI ones,
                         *   located between left shift and Y.
                         *   Produces GRAVE ACCENT and TILDE in a
                         *   US or UK Mac layout, REVERSE SOLIDUS
                         *   (backslash) and VERTICAL LINE in a
                         *   US or UK Windows layout, and
                         *   LESS-THAN SIGN and GREATER-THAN SIGN
                         *   in a Swiss German, German, or French
                         *   layout. */
  APPLICATION = 101,    /**< windows contextual menu, compose */
  POWER = 102,          /**< The USB document says this is a status flag,
                         *   not a physical key - but some Mac keyboards
                         *   do have a power key. */
  KP_EQUALS = 103,
  F13 = 104,
  F14 = 105,
  F15 = 106,
  F16 = 107,
  F17 = 108,
  F18 = 109,
  F19 = 110,
  F20 = 111,
  F21 = 112,
  F22 = 113,
  F23 = 114,
  F24 = 115,
  EXECUTE = 116,
  HELP = 117, /**< AL Integrated Help Center */
  MENU = 118, /**< Menu (show menu) */
  SELECT = 119,
  STOP = 120,  /**< AC Stop */
  AGAIN = 121, /**< AC Redo/Repeat */
  UNDO = 122,  /**< AC Undo */
  CUT = 123,   /**< AC Cut */
  COPY = 124,  /**< AC Copy */
  PASTE = 125, /**< AC Paste */
  FIND = 126,  /**< AC Find */
  MUTE = 127,
  VOLUMEUP = 128,
  VOLUMEDOWN = 129,
  /* not sure whether there's a reason to enable these */
  /*     LOCKINGCAPSLOCK = 130,  */
  /*     LOCKINGNUMLOCK = 131, */
  /*     LOCKINGSCROLLLOCK = 132, */
  KP_COMMA = 133,
  KP_EQUALSAS400 = 134,

  INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                          footnotes in USB doc */
  INTERNATIONAL2 = 136,
  INTERNATIONAL3 = 137, /**< Yen */
  INTERNATIONAL4 = 138,
  INTERNATIONAL5 = 139,
  INTERNATIONAL6 = 140,
  INTERNATIONAL7 = 141,
  INTERNATIONAL8 = 142,
  INTERNATIONAL9 = 143,
  LANG1 = 144, /**< Hangul/English toggle */
  LANG2 = 145, /**< Hanja conversion */
  LANG3 = 146, /**< Katakana */
  LANG4 = 147, /**< Hiragana */
  LANG5 = 148, /**< Zenkaku/Hankaku */
  LANG6 = 149, /**< reserved */
  LANG7 = 150, /**< reserved */
  LANG8 = 151, /**< reserved */
  LANG9 = 152, /**< reserved */

  ALTERASE = 153, /**< Erase-Eaze */
  SYSREQ = 154,
  CANCEL = 155, /**< AC Cancel */
  CLEAR = 156,
  PRIOR = 157,
  RETURN2 = 158,
  SEPARATOR = 159,
  OUTKEY = 160,
  OPER = 161,
  CLEARAGAIN = 162,
  CRSEL = 163,
  EXSEL = 164,

  KP_00 = 176,
  KP_000 = 177,
  THOUSANDSSEPARATOR = 178,
  DECIMALSEPARATOR = 179,
  CURRENCYUNIT = 180,
  CURRENCYSUBUNIT = 181,
  KP_LEFTPAREN = 182,
  KP_RIGHTPAREN = 183,
  KP_LEFTBRACE = 184,
  KP_RIGHTBRACE = 185,
  KP_TAB = 186,
  KP_BACKSPACE = 187,
  KP_A = 188,
  KP_B = 189,
  KP_C = 190,
  KP_D = 191,
  KP_E = 192,
  KP_F = 193,
  KP_XOR = 194,
  KP_POWER = 195,
  KP_PERCENT = 196,
  KP_LESS = 197,
  KP_GREATER = 198,
  KP_AMPERSAND = 199,
  KP_DBLAMPERSAND = 200,
  KP_VERTICALBAR = 201,
  KP_DBLVERTICALBAR = 202,
  KP_COLON = 203,
  KP_HASH = 204,
  KP_SPACE = 205,
  KP_AT = 206,
  KP_EXCLAM = 207,
  KP_MEMSTORE = 208,
  KP_MEMRECALL = 209,
  KP_MEMCLEAR = 210,
  KP_MEMADD = 211,
  KP_MEMSUBTRACT = 212,
  KP_MEMMULTIPLY = 213,
  KP_MEMDIVIDE = 214,
  KP_PLUSMINUS = 215,
  KP_CLEAR = 216,
  KP_CLEARENTRY = 217,
  KP_BINARY = 218,
  KP_OCTAL = 219,
  KP_DECIMAL = 220,
  KP_HEXADECIMAL = 221,

  LCTRL = 224,
  LSHIFT = 225,
  LALT = 226, /**< alt, option */
  LGUI = 227, /**< windows, command (apple), meta */
  RCTRL = 228,
  RSHIFT = 229,
  RALT = 230, /**< alt gr, option */
  RGUI = 231, /**< windows, command (apple), meta */

  MODE = 257, /**< I'm not sure if this is really not covered
               *   by any of the above, but since there's a
               *   special KMOD_MODE for it I'm adding it here
               */

  /* @} */ /* Usage page 0x07 */

  /**
   *  \name Usage page 0x0C
   *
   *  These values are mapped from usage page 0x0C (USB consumer page).
   *  See https://usb.org/sites/default/files/hut1_2.pdf
   *
   *  There are way more keys in the spec than we can represent in the
   *  current scancode range, so pick the ones that commonly come up in
   *  real world usage.
   */
  /* @{ */

  AUDIONEXT = 258,
  AUDIOPREV = 259,
  AUDIOSTOP = 260,
  AUDIOPLAY = 261,
  AUDIOMUTE = 262,
  MEDIASELECT = 263,
  WWW = 264, /**< AL Internet Browser */
  MAIL = 265,
  CALCULATOR = 266, /**< AL Calculator */
  COMPUTER = 267,
  AC_SEARCH = 268,    /**< AC Search */
  AC_HOME = 269,      /**< AC Home */
  AC_BACK = 270,      /**< AC Back */
  AC_FORWARD = 271,   /**< AC Forward */
  AC_STOP = 272,      /**< AC Stop */
  AC_REFRESH = 273,   /**< AC Refresh */
  AC_BOOKMARKS = 274, /**< AC Bookmarks */

  /* @} */ /* Usage page 0x0C */

  /**
   *  \name Walther keys
   *
   *  These are values that Christian Walther added (for mac keyboard?).
   */
  /* @{ */

  BRIGHTNESSDOWN = 275,
  BRIGHTNESSUP = 276,
  DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                         switch, video mode switch */
  KBDILLUMTOGGLE = 278,
  KBDILLUMDOWN = 279,
  KBDILLUMUP = 280,
  EJECT = 281,
  SLEEP = 282, /**< SC System Sleep */

  APP1 = 283,
  APP2 = 284,

  /* @} */ /* Walther keys */

  /**
   *  \name Usage page 0x0C (additional media keys)
   *
   *  These values are mapped from usage page 0x0C (USB consumer page).
   */
  /* @{ */

  AUDIOREWIND = 285,
  AUDIOFASTFORWARD = 286,

  /* @} */ /* Usage page 0x0C (additional media keys) */

  /**
   *  \name Mobile keys
   *
   *  These are values that are often used on mobile phones.
   */
  /* @{ */

  SOFTLEFT = 287,  /**< Usually situated below the display on phones and
                                     used as a multi-function feature key for
                      selecting  a software defined function shown on the bottom
                      left  of the display. */
  SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                     used as a multi-function feature key for
                      selecting a software defined function shown on the
                      bottom right of the display. */
  CALL = 289,      /**< Used for accepting phone calls. */
  ENDCALL = 290,   /**< Used for rejecting phone calls. */

  /* @} */ /* Mobile keys */

  /* Add any other keys here. */

  NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
                               for array bounds */
};

enum class VirtualCode : uint32_t {
  UNKNOWN = 0,

  RETURN = '\r',
  ESCAPE = '\x1B',
  BACKSPACE = '\b',
  TAB = '\t',
  SPACE = ' ',
  EXCLAIM = '!',
  QUOTEDBL = '"',
  HASH = '#',
  PERCENT = '%',
  DOLLAR = '$',
  AMPERSAND = '&',
  QUOTE = '\'',
  LEFTPAREN = '(',
  RIGHTPAREN = ')',
  ASTERISK = '*',
  PLUS = '+',
  COMMA = ',',
  MINUS = '-',
  PERIOD = '.',
  SLASH = '/',
  NUM_0 = '0',
  NUM_1 = '1',
  NUM_2 = '2',
  NUM_3 = '3',
  NUM_4 = '4',
  NUM_5 = '5',
  NUM_6 = '6',
  NUM_7 = '7',
  NUM_8 = '8',
  NUM_9 = '9',
  COLON = ':',
  SEMICOLON = ';',
  LESS = '<',
  EQUALS = '=',
  GREATER = '>',
  QUESTION = '?',
  AT = '@',

  /*
     Skip uppercase letters
   */

  LEFTBRACKET = '[',
  BACKSLASH = '\\',
  RIGHTBRACKET = ']',
  CARET = '^',
  UNDERSCORE = '_',
  BACKQUOTE = '`',
  a = 'a',
  b = 'b',
  c = 'c',
  d = 'd',
  e = 'e',
  f = 'f',
  g = 'g',
  h = 'h',
  i = 'i',
  j = 'j',
  k = 'k',
  l = 'l',
  m = 'm',
  n = 'n',
  o = 'o',
  p = 'p',
  q = 'q',
  r = 'r',
  s = 's',
  t = 't',
  u = 'u',
  v = 'v',
  w = 'w',
  x = 'x',
  y = 'y',
  z = 'z',

  CAPSLOCK = SCANCODE_TO_KEYCODE(ScanCode::CAPSLOCK),

  F1 = SCANCODE_TO_KEYCODE(ScanCode::F1),
  F2 = SCANCODE_TO_KEYCODE(ScanCode::F2),
  F3 = SCANCODE_TO_KEYCODE(ScanCode::F3),
  F4 = SCANCODE_TO_KEYCODE(ScanCode::F4),
  F5 = SCANCODE_TO_KEYCODE(ScanCode::F5),
  F6 = SCANCODE_TO_KEYCODE(ScanCode::F6),
  F7 = SCANCODE_TO_KEYCODE(ScanCode::F7),
  F8 = SCANCODE_TO_KEYCODE(ScanCode::F8),
  F9 = SCANCODE_TO_KEYCODE(ScanCode::F9),
  F10 = SCANCODE_TO_KEYCODE(ScanCode::F10),
  F11 = SCANCODE_TO_KEYCODE(ScanCode::F11),
  F12 = SCANCODE_TO_KEYCODE(ScanCode::F12),

  PRINTSCREEN = SCANCODE_TO_KEYCODE(ScanCode::PRINTSCREEN),
  SCROLLLOCK = SCANCODE_TO_KEYCODE(ScanCode::SCROLLLOCK),
  PAUSE = SCANCODE_TO_KEYCODE(ScanCode::PAUSE),
  INSERT = SCANCODE_TO_KEYCODE(ScanCode::INSERT),
  HOME = SCANCODE_TO_KEYCODE(ScanCode::HOME),
  PAGEUP = SCANCODE_TO_KEYCODE(ScanCode::PAGEUP),
  DEL = '\x7F',
  END = SCANCODE_TO_KEYCODE(ScanCode::END),
  PAGEDOWN = SCANCODE_TO_KEYCODE(ScanCode::PAGEDOWN),
  RIGHT = SCANCODE_TO_KEYCODE(ScanCode::RIGHT),
  LEFT = SCANCODE_TO_KEYCODE(ScanCode::LEFT),
  DOWN = SCANCODE_TO_KEYCODE(ScanCode::DOWN),
  UP = SCANCODE_TO_KEYCODE(ScanCode::UP),

  NUMLOCKCLEAR = SCANCODE_TO_KEYCODE(ScanCode::NUMLOCKCLEAR),
  KP_DIVIDE = SCANCODE_TO_KEYCODE(ScanCode::KP_DIVIDE),
  KP_MULTIPLY = SCANCODE_TO_KEYCODE(ScanCode::KP_MULTIPLY),
  KP_MINUS = SCANCODE_TO_KEYCODE(ScanCode::KP_MINUS),
  KP_PLUS = SCANCODE_TO_KEYCODE(ScanCode::KP_PLUS),
  KP_ENTER = SCANCODE_TO_KEYCODE(ScanCode::KP_ENTER),
  KP_1 = SCANCODE_TO_KEYCODE(ScanCode::KP_1),
  KP_2 = SCANCODE_TO_KEYCODE(ScanCode::KP_2),
  KP_3 = SCANCODE_TO_KEYCODE(ScanCode::KP_3),
  KP_4 = SCANCODE_TO_KEYCODE(ScanCode::KP_4),
  KP_5 = SCANCODE_TO_KEYCODE(ScanCode::KP_5),
  KP_6 = SCANCODE_TO_KEYCODE(ScanCode::KP_6),
  KP_7 = SCANCODE_TO_KEYCODE(ScanCode::KP_7),
  KP_8 = SCANCODE_TO_KEYCODE(ScanCode::KP_8),
  KP_9 = SCANCODE_TO_KEYCODE(ScanCode::KP_9),
  KP_0 = SCANCODE_TO_KEYCODE(ScanCode::KP_0),
  KP_PERIOD = SCANCODE_TO_KEYCODE(ScanCode::KP_PERIOD),

  APPLICATION = SCANCODE_TO_KEYCODE(ScanCode::APPLICATION),
  POWER = SCANCODE_TO_KEYCODE(ScanCode::POWER),
  KP_EQUALS = SCANCODE_TO_KEYCODE(ScanCode::KP_EQUALS),
  F13 = SCANCODE_TO_KEYCODE(ScanCode::F13),
  F14 = SCANCODE_TO_KEYCODE(ScanCode::F14),
  F15 = SCANCODE_TO_KEYCODE(ScanCode::F15),
  F16 = SCANCODE_TO_KEYCODE(ScanCode::F16),
  F17 = SCANCODE_TO_KEYCODE(ScanCode::F17),
  F18 = SCANCODE_TO_KEYCODE(ScanCode::F18),
  F19 = SCANCODE_TO_KEYCODE(ScanCode::F19),
  F20 = SCANCODE_TO_KEYCODE(ScanCode::F20),
  F21 = SCANCODE_TO_KEYCODE(ScanCode::F21),
  F22 = SCANCODE_TO_KEYCODE(ScanCode::F22),
  F23 = SCANCODE_TO_KEYCODE(ScanCode::F23),
  F24 = SCANCODE_TO_KEYCODE(ScanCode::F24),
  EXECUTE = SCANCODE_TO_KEYCODE(ScanCode::EXECUTE),
  HELP = SCANCODE_TO_KEYCODE(ScanCode::HELP),
  MENU = SCANCODE_TO_KEYCODE(ScanCode::MENU),
  SELECT = SCANCODE_TO_KEYCODE(ScanCode::SELECT),
  STOP = SCANCODE_TO_KEYCODE(ScanCode::STOP),
  AGAIN = SCANCODE_TO_KEYCODE(ScanCode::AGAIN),
  UNDO = SCANCODE_TO_KEYCODE(ScanCode::UNDO),
  CUT = SCANCODE_TO_KEYCODE(ScanCode::CUT),
  COPY = SCANCODE_TO_KEYCODE(ScanCode::COPY),
  PASTE = SCANCODE_TO_KEYCODE(ScanCode::PASTE),
  FIND = SCANCODE_TO_KEYCODE(ScanCode::FIND),
  MUTE = SCANCODE_TO_KEYCODE(ScanCode::MUTE),
  VOLUMEUP = SCANCODE_TO_KEYCODE(ScanCode::VOLUMEUP),
  VOLUMEDOWN = SCANCODE_TO_KEYCODE(ScanCode::VOLUMEDOWN),
  KP_COMMA = SCANCODE_TO_KEYCODE(ScanCode::KP_COMMA),
  KP_EQUALSAS400 = SCANCODE_TO_KEYCODE(ScanCode::KP_EQUALSAS400),

  ALTERASE = SCANCODE_TO_KEYCODE(ScanCode::ALTERASE),
  SYSREQ = SCANCODE_TO_KEYCODE(ScanCode::SYSREQ),
  CANCEL = SCANCODE_TO_KEYCODE(ScanCode::CANCEL),
  CLEAR = SCANCODE_TO_KEYCODE(ScanCode::CLEAR),
  PRIOR = SCANCODE_TO_KEYCODE(ScanCode::PRIOR),
  RETURN2 = SCANCODE_TO_KEYCODE(ScanCode::RETURN2),
  SEPARATOR = SCANCODE_TO_KEYCODE(ScanCode::SEPARATOR),
  OUTKEY = SCANCODE_TO_KEYCODE(ScanCode::OUTKEY),
  OPER = SCANCODE_TO_KEYCODE(ScanCode::OPER),
  CLEARAGAIN = SCANCODE_TO_KEYCODE(ScanCode::CLEARAGAIN),
  CRSEL = SCANCODE_TO_KEYCODE(ScanCode::CRSEL),
  EXSEL = SCANCODE_TO_KEYCODE(ScanCode::EXSEL),

  KP_00 = SCANCODE_TO_KEYCODE(ScanCode::KP_00),
  KP_000 = SCANCODE_TO_KEYCODE(ScanCode::KP_000),
  THOUSANDSSEPARATOR = SCANCODE_TO_KEYCODE(ScanCode::THOUSANDSSEPARATOR),
  DECIMALSEPARATOR = SCANCODE_TO_KEYCODE(ScanCode::DECIMALSEPARATOR),
  CURRENCYUNIT = SCANCODE_TO_KEYCODE(ScanCode::CURRENCYUNIT),
  CURRENCYSUBUNIT = SCANCODE_TO_KEYCODE(ScanCode::CURRENCYSUBUNIT),
  KP_LEFTPAREN = SCANCODE_TO_KEYCODE(ScanCode::KP_LEFTPAREN),
  KP_RIGHTPAREN = SCANCODE_TO_KEYCODE(ScanCode::KP_RIGHTPAREN),
  KP_LEFTBRACE = SCANCODE_TO_KEYCODE(ScanCode::KP_LEFTBRACE),
  KP_RIGHTBRACE = SCANCODE_TO_KEYCODE(ScanCode::KP_RIGHTBRACE),
  KP_TAB = SCANCODE_TO_KEYCODE(ScanCode::KP_TAB),
  KP_BACKSPACE = SCANCODE_TO_KEYCODE(ScanCode::KP_BACKSPACE),
  KP_A = SCANCODE_TO_KEYCODE(ScanCode::KP_A),
  KP_B = SCANCODE_TO_KEYCODE(ScanCode::KP_B),
  KP_C = SCANCODE_TO_KEYCODE(ScanCode::KP_C),
  KP_D = SCANCODE_TO_KEYCODE(ScanCode::KP_D),
  KP_E = SCANCODE_TO_KEYCODE(ScanCode::KP_E),
  KP_F = SCANCODE_TO_KEYCODE(ScanCode::KP_F),
  KP_XOR = SCANCODE_TO_KEYCODE(ScanCode::KP_XOR),
  KP_POWER = SCANCODE_TO_KEYCODE(ScanCode::KP_POWER),
  KP_PERCENT = SCANCODE_TO_KEYCODE(ScanCode::KP_PERCENT),
  KP_LESS = SCANCODE_TO_KEYCODE(ScanCode::KP_LESS),
  KP_GREATER = SCANCODE_TO_KEYCODE(ScanCode::KP_GREATER),
  KP_AMPERSAND = SCANCODE_TO_KEYCODE(ScanCode::KP_AMPERSAND),
  KP_DBLAMPERSAND = SCANCODE_TO_KEYCODE(ScanCode::KP_DBLAMPERSAND),
  KP_VERTICALBAR = SCANCODE_TO_KEYCODE(ScanCode::KP_VERTICALBAR),
  KP_DBLVERTICALBAR = SCANCODE_TO_KEYCODE(ScanCode::KP_DBLVERTICALBAR),
  KP_COLON = SCANCODE_TO_KEYCODE(ScanCode::KP_COLON),
  KP_HASH = SCANCODE_TO_KEYCODE(ScanCode::KP_HASH),
  KP_SPACE = SCANCODE_TO_KEYCODE(ScanCode::KP_SPACE),
  KP_AT = SCANCODE_TO_KEYCODE(ScanCode::KP_AT),
  KP_EXCLAM = SCANCODE_TO_KEYCODE(ScanCode::KP_EXCLAM),
  KP_MEMSTORE = SCANCODE_TO_KEYCODE(ScanCode::KP_MEMSTORE),
  KP_MEMRECALL = SCANCODE_TO_KEYCODE(ScanCode::KP_MEMRECALL),
  KP_MEMCLEAR = SCANCODE_TO_KEYCODE(ScanCode::KP_MEMCLEAR),
  KP_MEMADD = SCANCODE_TO_KEYCODE(ScanCode::KP_MEMADD),
  KP_MEMSUBTRACT = SCANCODE_TO_KEYCODE(ScanCode::KP_MEMSUBTRACT),
  KP_MEMMULTIPLY = SCANCODE_TO_KEYCODE(ScanCode::KP_MEMMULTIPLY),
  KP_MEMDIVIDE = SCANCODE_TO_KEYCODE(ScanCode::KP_MEMDIVIDE),
  KP_PLUSMINUS = SCANCODE_TO_KEYCODE(ScanCode::KP_PLUSMINUS),
  KP_CLEAR = SCANCODE_TO_KEYCODE(ScanCode::KP_CLEAR),
  KP_CLEARENTRY = SCANCODE_TO_KEYCODE(ScanCode::KP_CLEARENTRY),
  KP_BINARY = SCANCODE_TO_KEYCODE(ScanCode::KP_BINARY),
  KP_OCTAL = SCANCODE_TO_KEYCODE(ScanCode::KP_OCTAL),
  KP_DECIMAL = SCANCODE_TO_KEYCODE(ScanCode::KP_DECIMAL),
  KP_HEXADECIMAL = SCANCODE_TO_KEYCODE(ScanCode::KP_HEXADECIMAL),

  LCTRL = SCANCODE_TO_KEYCODE(ScanCode::LCTRL),
  LSHIFT = SCANCODE_TO_KEYCODE(ScanCode::LSHIFT),
  LALT = SCANCODE_TO_KEYCODE(ScanCode::LALT),
  LGUI = SCANCODE_TO_KEYCODE(ScanCode::LGUI),
  RCTRL = SCANCODE_TO_KEYCODE(ScanCode::RCTRL),
  RSHIFT = SCANCODE_TO_KEYCODE(ScanCode::RSHIFT),
  RALT = SCANCODE_TO_KEYCODE(ScanCode::RALT),
  RGUI = SCANCODE_TO_KEYCODE(ScanCode::RGUI),

  MODE = SCANCODE_TO_KEYCODE(ScanCode::MODE),

  AUDIONEXT = SCANCODE_TO_KEYCODE(ScanCode::AUDIONEXT),
  AUDIOPREV = SCANCODE_TO_KEYCODE(ScanCode::AUDIOPREV),
  AUDIOSTOP = SCANCODE_TO_KEYCODE(ScanCode::AUDIOSTOP),
  AUDIOPLAY = SCANCODE_TO_KEYCODE(ScanCode::AUDIOPLAY),
  AUDIOMUTE = SCANCODE_TO_KEYCODE(ScanCode::AUDIOMUTE),
  MEDIASELECT = SCANCODE_TO_KEYCODE(ScanCode::MEDIASELECT),
  WWW = SCANCODE_TO_KEYCODE(ScanCode::WWW),
  MAIL = SCANCODE_TO_KEYCODE(ScanCode::MAIL),
  CALCULATOR = SCANCODE_TO_KEYCODE(ScanCode::CALCULATOR),
  COMPUTER = SCANCODE_TO_KEYCODE(ScanCode::COMPUTER),
  AC_SEARCH = SCANCODE_TO_KEYCODE(ScanCode::AC_SEARCH),
  AC_HOME = SCANCODE_TO_KEYCODE(ScanCode::AC_HOME),
  AC_BACK = SCANCODE_TO_KEYCODE(ScanCode::AC_BACK),
  AC_FORWARD = SCANCODE_TO_KEYCODE(ScanCode::AC_FORWARD),
  AC_STOP = SCANCODE_TO_KEYCODE(ScanCode::AC_STOP),
  AC_REFRESH = SCANCODE_TO_KEYCODE(ScanCode::AC_REFRESH),
  AC_BOOKMARKS = SCANCODE_TO_KEYCODE(ScanCode::AC_BOOKMARKS),

  BRIGHTNESSDOWN = SCANCODE_TO_KEYCODE(ScanCode::BRIGHTNESSDOWN),
  BRIGHTNESSUP = SCANCODE_TO_KEYCODE(ScanCode::BRIGHTNESSUP),
  DISPLAYSWITCH = SCANCODE_TO_KEYCODE(ScanCode::DISPLAYSWITCH),
  KBDILLUMTOGGLE = SCANCODE_TO_KEYCODE(ScanCode::KBDILLUMTOGGLE),
  KBDILLUMDOWN = SCANCODE_TO_KEYCODE(ScanCode::KBDILLUMDOWN),
  KBDILLUMUP = SCANCODE_TO_KEYCODE(ScanCode::KBDILLUMUP),
  EJECT = SCANCODE_TO_KEYCODE(ScanCode::EJECT),
  SLEEP = SCANCODE_TO_KEYCODE(ScanCode::SLEEP),
  APP1 = SCANCODE_TO_KEYCODE(ScanCode::APP1),
  APP2 = SCANCODE_TO_KEYCODE(ScanCode::APP2),

  AUDIOREWIND = SCANCODE_TO_KEYCODE(ScanCode::AUDIOREWIND),
  AUDIOFASTFORWARD = SCANCODE_TO_KEYCODE(ScanCode::AUDIOFASTFORWARD),

  SOFTLEFT = SCANCODE_TO_KEYCODE(ScanCode::SOFTLEFT),
  SOFTRIGHT = SCANCODE_TO_KEYCODE(ScanCode::SOFTRIGHT),
  CALL = SCANCODE_TO_KEYCODE(ScanCode::CALL),
  ENDCALL = SCANCODE_TO_KEYCODE(ScanCode::ENDCALL)
};

enum class MouseButton { Left, Middle, Right, X1, X2 };

} // namespace GEngine