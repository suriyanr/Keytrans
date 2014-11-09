// Function prototypes.
void log_string();
void write_default_map();
const char *VKCodeToString(unsigned char VKCode);
unsigned char VKStringToCode(char *VKString);
const char *BucketCodeToString(unsigned char BucketCode);
unsigned char BucketStringToCode(char *BucketString);
void initialise_map();
unsigned char scan_code(unsigned char vk);
int CheckVKCode(unsigned char vk);
int TranslateVKCode(unsigned char vk, unsigned char key[6], unsigned char s_key[6]);
int TranslateLigature(unsigned char LMap[], unsigned char key[6], unsigned char s_key[6]);


#define VK_OEM_1      0xBA
#define VK_OEM_2      0xBF
#define VK_OEM_3      0xC0
#define VK_OEM_4      0xDB
#define VK_OEM_5      0xDC
#define VK_OEM_6      0xDD
#define VK_OEM_7      0xDE
#define VK_OEM_MINUS  0xBD
#define VK_OEM_PLUS   0xBB
#define VK_OEM_COMMA  0xBC
#define VK_OEM_PERIOD 0xBE
#define VK_NUMPAD0    0x60


// Below is all the Virtual Keys that we are interested in.
// We have four rows of keys as below:
// First  ROW: `1234567890-=
// Second ROW: qwertyuiop[]\
// Third  ROW: asdfghjkl;'
// Fourth ROW: zxcvbnm,./

// The names below will appear in the ini file for the association.

// First Row.
#define VK_APOSTROPHE VK_OEM_3
#define VK_0          0x30
#define VK_1          0x31
#define VK_2          0x32
#define VK_3          0x33
#define VK_4          0x34
#define VK_5          0x35
#define VK_6          0x36
#define VK_7          0x37
#define VK_8          0x38
#define VK_9          0x39
#define VK_MINUS      VK_OEM_MINUS
#define VK_EQUAL      VK_OEM_PLUS

// Second Row.
#define VK_Q          0x51
#define VK_W          0x57
#define VK_E          0x45
#define VK_R          0x52
#define VK_T          0x54
#define VK_Y          0x59
#define VK_U          0x55
#define VK_I          0x49
#define VK_O          0x4F
#define VK_P          0x50
#define VK_BRA_OPEN   VK_OEM_4
#define VK_BRA_CLOSE  VK_OEM_6
#define VK_BACK_SLASH VK_OEM_5

// Third Row.
#define VK_A          0x41
#define VK_S          0x53
#define VK_D          0x44
#define VK_F          0x46
#define VK_G          0x47
#define VK_H          0x48
#define VK_J          0x4A
#define VK_K          0x4B
#define VK_L          0x4C
#define VK_COLON      VK_OEM_1
#define VK_QUOTES     VK_OEM_7

// Fourth Row.
#define VK_Z          0x5A
#define VK_X          0x58
#define VK_C          0x43
#define VK_V          0x56
#define VK_B          0x42
#define VK_N          0x4E
#define VK_M          0x4D
#define VK_COMMA      VK_OEM_COMMA
#define VK_DOT        VK_OEM_PERIOD
#define VK_FOR_SLASH  VK_OEM_2

#define VK_NULL       0x00

// Names of the above as they appear in the INI file.
// First Row.
#define STR_VK_APOSTROPHE "VK_APOSTROPHE"
#define STR_VK_0          "VK_0"
#define STR_VK_1          "VK_1"
#define STR_VK_2          "VK_2"
#define STR_VK_3          "VK_3"
#define STR_VK_4          "VK_4"
#define STR_VK_5          "VK_5"
#define STR_VK_6          "VK_6"
#define STR_VK_7          "VK_7"
#define STR_VK_8          "VK_8"
#define STR_VK_9          "VK_9"
#define STR_VK_MINUS      "VK_MINUS"
#define STR_VK_EQUAL      "VK_EQUAL"

// Second Row.
#define STR_VK_Q          "VK_Q"
#define STR_VK_W          "VK_W"
#define STR_VK_E          "VK_E"
#define STR_VK_R          "VK_R"
#define STR_VK_T          "VK_T"
#define STR_VK_Y          "VK_Y"
#define STR_VK_U          "VK_U"
#define STR_VK_I          "VK_I"
#define STR_VK_O          "VK_O"
#define STR_VK_P          "VK_P"
#define STR_VK_BRA_OPEN   "VK_BRA_OPEN"
#define STR_VK_BRA_CLOSE  "VK_BRA_CLOSE"
#define STR_VK_BACK_SLASH "VK_BACK_SLASH"

// Third Row.
#define STR_VK_A          "VK_A"
#define STR_VK_S          "VK_S"
#define STR_VK_D          "VK_D"
#define STR_VK_F          "VK_F"
#define STR_VK_G          "VK_G"
#define STR_VK_H          "VK_H"
#define STR_VK_J          "VK_J"
#define STR_VK_K          "VK_K"
#define STR_VK_L          "VK_L"
#define STR_VK_COLON      "VK_COLON"
#define STR_VK_QUOTES     "VK_QUOTES"

// Fourth Row.
#define STR_VK_Z          "VK_Z"
#define STR_VK_X          "VK_X"
#define STR_VK_C          "VK_C"
#define STR_VK_V          "VK_V"
#define STR_VK_B          "VK_N"
#define STR_VK_N          "VK_N"
#define STR_VK_M          "VK_M"
#define STR_VK_COMMA      "VK_COMMA"
#define STR_VK_DOT        "VK_DOT"
#define STR_VK_FOR_SLASH  "VK_FOR_SLASH"

#define STR_VK_NULL       ""

// String array for reading the ini file translations.
char *IniStrArray[] = {
   STR_VK_APOSTROPHE,
   STR_VK_0,
   STR_VK_1,
   STR_VK_2,
   STR_VK_3,
   STR_VK_4,
   STR_VK_5,
   STR_VK_6,
   STR_VK_7,
   STR_VK_8,
   STR_VK_9,
   STR_VK_MINUS,
   STR_VK_EQUAL,

   STR_VK_Q,
   STR_VK_W,
   STR_VK_E,
   STR_VK_R,
   STR_VK_T,
   STR_VK_Y,
   STR_VK_U,
   STR_VK_I,
   STR_VK_O,
   STR_VK_P,
   STR_VK_BRA_OPEN, 
   STR_VK_BRA_CLOSE,
   STR_VK_BACK_SLASH,

   STR_VK_A,
   STR_VK_S,
   STR_VK_D,
   STR_VK_F,
   STR_VK_G,
   STR_VK_H,
   STR_VK_J,
   STR_VK_K,
   STR_VK_L,
   STR_VK_COLON,
   STR_VK_QUOTES,

   STR_VK_Z,
   STR_VK_X,
   STR_VK_C,
   STR_VK_V,
   STR_VK_B,
   STR_VK_N,
   STR_VK_M,
   STR_VK_COMMA,
   STR_VK_DOT,
   STR_VK_FOR_SLASH,

   STR_VK_NULL
};


// Below is the translation to be performed for each key listed above.
// Form: #define TR_VK_... value (Alt 1/2/3/4/5/6 digits).
// 0s at the end should be discarded while Alt playing the keys.
// First = length of string., string (max 6) 
// Below is the default, tuned for VUTAM, incase no INI file present.
// If INI file is present, we load the array from that.
#define TR_VK_APOSTROPHE 3, 1, 4, 8, 0, 0, 0
#define TR_VK_1          4, 0, 2, 4, 5, 0, 0
#define TR_VK_2          3, 1, 6, 3, 0, 0, 0
#define TR_VK_3          3, 1, 5, 1, 0, 0, 0
#define TR_VK_4          3, 1, 5, 0, 0, 0, 0
#define TR_VK_5          3, 1, 2, 9, 0, 0, 0
#define TR_VK_6          4, 0, 1, 3, 4, 0, 0
#define TR_VK_7          3, 2, 4, 8, 0, 0, 0
#define TR_VK_8          3, 1, 5, 5, 0, 0, 0
#define TR_VK_9          3, 1, 5, 6, 0, 0, 0
#define TR_VK_0          2, 2, 1, 0, 0, 0, 0
#define TR_VK_MINUS      4, 0, 1, 4, 9, 0, 0
#define TR_VK_EQUAL      2, 2, 0, 0, 0, 0, 0

#define TR_VK_Q          3, 1, 4, 2, 0, 0, 0
#define TR_VK_W          3, 1, 4, 3, 0, 0, 0
#define TR_VK_E          3, 1, 2, 8, 0, 0, 0
#define TR_VK_R          3, 1, 4, 4, 0, 0, 0
#define TR_VK_T          3, 1, 6, 5, 0, 0, 0
#define TR_VK_Y          3, 1, 5, 3, 0, 0, 0
#define TR_VK_U          3, 1, 5, 4, 0, 0, 0
#define TR_VK_I          3, 1, 6, 0, 0, 0, 0
#define TR_VK_O          3, 1, 3, 3, 0, 0, 0
#define TR_VK_P          3, 1, 3, 1, 0, 0, 0
#define TR_VK_BRA_OPEN   3, 2, 2, 5, 0, 0, 0
#define TR_VK_BRA_CLOSE  4, 0, 1, 7, 4, 0, 0
#define TR_VK_BACK_SLASH 4, 0, 1, 6, 9, 0, 0

#define TR_VK_A          3, 1, 3, 2, 0, 0, 0
#define TR_VK_S          4, 0, 2, 2, 7, 0, 0
#define TR_VK_D          3, 1, 3, 4, 0, 0, 0
#define TR_VK_F          3, 1, 3, 5, 0, 0, 0
#define TR_VK_G          3, 1, 3, 0, 0, 0, 0
#define TR_VK_H          3, 1, 3, 8, 0, 0, 0
#define TR_VK_J          3, 1, 3, 6, 0, 0, 0
#define TR_VK_K          3, 1, 3, 7, 0, 0, 0
#define TR_VK_L          3, 1, 6, 1, 0, 0, 0
#define TR_VK_COLON      4, 0, 1, 5, 3, 0, 0
#define TR_VK_QUOTES     4, 0, 1, 8, 0, 0, 0

#define TR_VK_Z          3, 1, 4, 1, 0, 0, 0
#define TR_VK_X          3, 1, 4, 0, 0, 0, 0
#define TR_VK_C          3, 1, 3, 9, 0, 0, 0
#define TR_VK_V          3, 1, 6, 4, 0, 0, 0
#define TR_VK_B          3, 1, 6, 2, 0, 0, 0
#define TR_VK_N          3, 1, 4, 9, 0, 0, 0
#define TR_VK_M          3, 1, 4, 7, 0, 0, 0
#define TR_VK_COMMA      4, 0, 1, 6, 8, 0, 0
#define TR_VK_DOT        3, 1, 4, 6, 0, 0, 0
#define TR_VK_FOR_SLASH  3, 2, 6, 6, 0, 0, 0

#define TR_VK_NULL       0, 0, 0, 0, 0, 0, 0

unsigned char KeyTrans[][8] =
{
  { VK_APOSTROPHE, TR_VK_APOSTROPHE },
  { VK_1, TR_VK_1 },
  { VK_2, TR_VK_2 },
  { VK_3, TR_VK_3 },
  { VK_4, TR_VK_4 },
  { VK_5, TR_VK_5 },
  { VK_6, TR_VK_6 },
  { VK_7, TR_VK_7 },
  { VK_8, TR_VK_8 },
  { VK_9, TR_VK_9 },
  { VK_0, TR_VK_0 },
  { VK_MINUS, TR_VK_MINUS },
  { VK_EQUAL, TR_VK_EQUAL },

  { VK_Q, TR_VK_Q },
  { VK_W, TR_VK_W },
  { VK_E, TR_VK_E },
  { VK_R, TR_VK_R },
  { VK_T, TR_VK_T },
  { VK_Y, TR_VK_Y },
  { VK_U, TR_VK_U },
  { VK_I, TR_VK_I },
  { VK_O, TR_VK_O },
  { VK_P, TR_VK_P },
  { VK_BRA_OPEN, TR_VK_BRA_OPEN },
  { VK_BRA_CLOSE, TR_VK_BRA_CLOSE },
  { VK_BACK_SLASH, TR_VK_BACK_SLASH },

  { VK_A, TR_VK_A },
  { VK_S, TR_VK_S },
  { VK_D, TR_VK_D },
  { VK_F, TR_VK_F },
  { VK_G, TR_VK_G },
  { VK_H, TR_VK_H },
  { VK_J, TR_VK_J },
  { VK_K, TR_VK_K },
  { VK_L, TR_VK_L },
  { VK_COLON, TR_VK_COLON },
  { VK_QUOTES, TR_VK_QUOTES },

  { VK_Z, TR_VK_Z },
  { VK_X, TR_VK_X },
  { VK_C, TR_VK_C },
  { VK_V, TR_VK_V },
  { VK_B, TR_VK_B },
  { VK_N, TR_VK_N },
  { VK_M, TR_VK_M },
  { VK_COMMA, TR_VK_COMMA },
  { VK_DOT, TR_VK_DOT },
  { VK_FOR_SLASH, TR_VK_FOR_SLASH },

  { VK_NULL, TR_VK_NULL } 
};

// Below we always need so that we pass the scan code along with the
// virtual key code. Note, although the MS doc says Scan code is ignored,
// if we pass 0 in keybd_event(), the application does not register
// any character sequences.
unsigned char VKScan[][2] = {
  { VK_NUMPAD0, 0x52 },
  { VK_NUMPAD0 + 1, 0x4F },
  { VK_NUMPAD0 + 2, 0x50 },
  { VK_NUMPAD0 + 3, 0x51 },
  { VK_NUMPAD0 + 4, 0x4B },
  { VK_NUMPAD0 + 5, 0x4C },
  { VK_NUMPAD0 + 6, 0x4D },
  { VK_NUMPAD0 + 7, 0x47 },
  { VK_NUMPAD0 + 8, 0x48 },
  { VK_NUMPAD0 + 9, 0x49 },
  { VK_NULL, 0 }
};

#define STR_BUCKET1       "BUCKET1"
#define STR_BUCKET2       "BUCKET2"
#define STR_BUCKET3       "BUCKET3"
#define BUCKET1           1
#define BUCKET2           2
#define BUCKET3           3

// Below is the Ligature mapping.
// Max character combination to form one character is 3.
// Format is as follows:
// bucket level, VK, bucket level, VK, ... , 0, 
// count, alt char 1, alt char 2, alt char 3, max 6
// Max of 250 Ligatures as of now (as required by Version 2)
// Note [even][0..6] is the keypress history.
// and [even + 1][0..6] is the translation of Ligature.
#define LIGATURES_MAX    250
unsigned char LigatureMap[2 * LIGATURES_MAX][7] = {
  { BUCKET1, VK_T, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 4, 4, 0 },

  { BUCKET1, VK_K, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 4, 5, 0 },

  { BUCKET1, VK_R, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 4, 6, 0 },

  { BUCKET1, VK_M, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 4, 7, 0 },

  { BUCKET1, VK_N, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 4, 8, 0 },

  { BUCKET1, VK_P, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 4, 9, 0 },

  { BUCKET1, VK_D, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 5, 0, 0 },

  { BUCKET1, VK_V, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 5, 1, 0 },

  { BUCKET1, VK_L, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 5, 2, 0 },

  { BUCKET1, VK_W, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 5, 3, 0 },

  { BUCKET1, VK_X, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 5, 4, 0 },

  { BUCKET1, VK_Q, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 5, 5, 0 },

  { BUCKET1, VK_C, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 5, 6, 0 },

  { BUCKET1, VK_F, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 5, 7, 0 },

  { BUCKET1, VK_B, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 5, 8, 0 },

  { BUCKET1, VK_G, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 5, 9, 0 },

  { BUCKET1, VK_Z, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 6, 0, 0 },

  { BUCKET1, VK_BACK_SLASH, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 6, 1, 0 },

  { BUCKET1, VK_J, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 6, 2, 0 },

  { BUCKET1, VK_S, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 6, 3, 0 },

  { BUCKET2, VK_S, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 6, 4, 0 },

  { BUCKET1, VK_H, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 6, 5, 0 },

  { BUCKET2, VK_J, BUCKET1, VK_QUOTES, 0, 0, 0 },
  { 5, 5, 7, 3, 6, 6, 0 },
  /* Above are for character + . */

  { BUCKET1, VK_T, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 6, 7, 0 },

  { BUCKET1, VK_K, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 6, 8, 0 },

  { BUCKET1, VK_R, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 6, 9, 0 },

  { BUCKET1, VK_M, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 7, 0, 0 },

  { BUCKET1, VK_N, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 7, 1, 0 },

  { BUCKET1, VK_P, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 7, 2, 0 },

  { BUCKET1, VK_D, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 7, 3, 0 },

  { BUCKET1, VK_V, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 7, 4, 0 },

  { BUCKET1, VK_L, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 7, 5, 0 },

  { BUCKET1, VK_W, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 7, 6, 0 },

  { BUCKET1, VK_X, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 7, 7, 0 },

  { BUCKET1, VK_Q, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 7, 8, 0 },

  { BUCKET1, VK_C, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 7, 9, 0 },

  { BUCKET1, VK_F, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 8, 0, 0 },

  { BUCKET1, VK_B, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 8, 1, 0 },

  { BUCKET1, VK_G, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 8, 2, 0 },

  { BUCKET1, VK_Z, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 8, 3, 0 },

  { BUCKET1, VK_BACK_SLASH, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 8, 4, 0 },

  { BUCKET1, VK_J, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 8, 5, 0 },

  { BUCKET1, VK_S, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 8, 6, 0 },

  { BUCKET2, VK_S, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 8, 7, 0 },

  { BUCKET1, VK_H, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 8, 8, 0 },

  { BUCKET2, VK_J, BUCKET1, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 3, 8, 9, 0 },
  /* Above are for character + ] */

  { BUCKET1, VK_T, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 3, 9, 0, 0 },

  { BUCKET1, VK_K, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 3, 9, 1, 0 },

  { BUCKET1, VK_R, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 3, 9, 2, 0 },

  { BUCKET1, VK_M, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 3, 9, 3, 0 },

  { BUCKET1, VK_N, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 3, 9, 4, 0 },

  { BUCKET1, VK_P, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 3, 9, 5, 0 },

  { BUCKET1, VK_D, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 3, 9, 6, 0 },

  { BUCKET1, VK_V, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 3, 9, 7, 0 },

  { BUCKET1, VK_L, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 3, 9, 8, 0 },

  { BUCKET1, VK_W, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 3, 9, 9, 0 },

  { BUCKET1, VK_X, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 0, 0, 0 },

  { BUCKET1, VK_Q, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 0, 1, 0 },

  { BUCKET1, VK_C, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 0, 2, 0 },

  { BUCKET1, VK_F, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 0, 3, 0 },

  { BUCKET1, VK_B, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 0, 4, 0 },

  { BUCKET1, VK_G, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 0, 5, 0 },

  { BUCKET1, VK_Z, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 0, 6, 0 },

  { BUCKET1, VK_BACK_SLASH, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 0, 7, 0 },

  { BUCKET1, VK_J, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 0, 8, 0 },

  { BUCKET1, VK_S, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 0, 9, 0 },

  { BUCKET2, VK_S, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 1, 0, 0 },

  { BUCKET1, VK_H, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 1, 1, 0 },

  { BUCKET2, VK_J, BUCKET1, VK_FOR_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 1, 2, 0 },
  /* Ligature Character + / */

  { BUCKET2, VK_I, BUCKET1, VK_T, 0, 0, 0 },
  { 5, 5, 7, 4, 1, 3, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_K, 0, 0, 0 },
  { 5, 5, 7, 4, 1, 4, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_R, 0, 0, 0 },
  { 5, 5, 7, 4, 1, 5, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_M, 0, 0, 0 },
  { 5, 5, 7, 4, 1, 6, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_N, 0, 0, 0 },
  { 5, 5, 7, 4, 1, 7, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_P, 0, 0, 0 },
  { 5, 5, 7, 4, 1, 8, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_D, 0, 0, 0 },
  { 5, 5, 7, 4, 1, 9, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_V, 0, 0, 0 },
  { 5, 5, 7, 4, 2, 0, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_L, 0, 0, 0 },
  { 5, 5, 7, 4, 2, 1, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_W, 0, 0, 0 },
  { 5, 5, 7, 4, 2, 2, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_X, 0, 0, 0 },
  { 5, 5, 7, 4, 2, 3, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_Q, 0, 0, 0 },
  { 5, 5, 7, 4, 2, 4, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_C, 0, 0, 0 },
  { 5, 5, 7, 4, 2, 5, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_F, 0, 0, 0 },
  { 5, 5, 7, 4, 2, 6, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_B, 0, 0, 0 },
  { 5, 5, 7, 4, 2, 7, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_G, 0, 0, 0 },
  { 5, 5, 7, 4, 2, 8, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_Z, 0, 0, 0 },
  { 5, 5, 7, 4, 2, 9, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_BACK_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 3, 0, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_J, 0, 0, 0 },
  { 5, 5, 7, 4, 3, 1, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_S, 0, 0, 0 },
  { 5, 5, 7, 4, 3, 2, 0 },

  { BUCKET2, VK_I, BUCKET2, VK_S, 0, 0, 0 },
  { 5, 5, 7, 4, 3, 3, 0 },

  { BUCKET2, VK_I, BUCKET1, VK_H, 0, 0, 0 },
  { 5, 5, 7, 4, 3, 4, 0 },

  { BUCKET2, VK_I, BUCKET2, VK_J, 0, 0, 0 },
  { 5, 5, 7, 4, 3, 5, 0 },
  /* Ligature I + character */


  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_T, 0, 0, 0 },
  { 5, 5, 7, 4, 3, 6, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_K, 0, 0, 0 },
  { 5, 5, 7, 4, 3, 7, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_R, 0, 0, 0 },
  { 5, 5, 7, 4, 3, 8, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_M, 0, 0, 0 },
  { 5, 5, 7, 4, 3, 9, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_N, 0, 0, 0 },
  { 5, 5, 7, 4, 4, 0, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_P, 0, 0, 0 },
  { 5, 5, 7, 4, 4, 1, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_D, 0, 0, 0 },
  { 5, 5, 7, 4, 4, 2, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_V, 0, 0, 0 },
  { 5, 5, 7, 4, 4, 3, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_L, 0, 0, 0 },
  { 5, 5, 7, 4, 4, 4, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_W, 0, 0, 0 },
  { 5, 5, 7, 4, 4, 5, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_X, 0, 0, 0 },
  { 5, 5, 7, 4, 4, 6, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_Q, 0, 0, 0 },
  { 5, 5, 7, 4, 4, 7, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_C, 0, 0, 0 },
  { 5, 5, 7, 4, 4, 8, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_F, 0, 0, 0 },
  { 5, 5, 7, 4, 4, 9, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_B, 0, 0, 0 },
  { 5, 5, 7, 4, 5, 0, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_G, 0, 0, 0 },
  { 5, 5, 7, 4, 5, 1, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_Z, 0, 0, 0 },
  { 5, 5, 7, 4, 5, 2, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_BACK_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 5, 3, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_J, 0, 0, 0 },
  { 5, 5, 7, 4, 5, 4, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_S, 0, 0, 0 },
  { 5, 5, 7, 4, 5, 5, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET2, VK_S, 0, 0, 0 },
  { 5, 5, 7, 4, 5, 6, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_H, 0, 0, 0 },
  { 5, 5, 7, 4, 5, 7, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET2, VK_J, 0, 0, 0 },
  { 5, 5, 7, 4, 5, 8, 0 },
  /* Ligature [ + character */


  { BUCKET1, VK_J, BUCKET2, VK_H, 0, 0, 0 },
  { 5, 5, 7, 4, 5, 9, 0 },

  { BUCKET1, VK_S, BUCKET2, VK_H, 0, 0, 0 },
  { 5, 5, 7, 4, 6, 0, 0 },

  { BUCKET2, VK_S, BUCKET2, VK_H, 0, 0, 0 },
  { 5, 5, 7, 4, 6, 1, 0 },

  { BUCKET1, VK_H, BUCKET2, VK_H, 0, 0, 0 },
  { 5, 5, 7, 4, 6, 2, 0 },

  { BUCKET2, VK_J, BUCKET2, VK_H, 0, 0, 0 },
  { 5, 5, 7, 4, 6, 3, 0 },
  /* Ligature character + H */

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_T, 0, 0, 0 },
  { 5, 5, 7, 4, 6, 4, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_K, 0, 0, 0 },
  { 5, 5, 7, 4, 6, 5, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_R, 0, 0, 0 },
  { 5, 5, 7, 4, 6, 6, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_M, 0, 0, 0 },
  { 5, 5, 7, 4, 6, 7, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_N, 0, 0, 0 },
  { 5, 5, 7, 4, 6, 8, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_P, 0, 0, 0 },
  { 5, 5, 7, 4, 6, 9, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_D, 0, 0, 0 },
  { 5, 5, 7, 4, 7, 0, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_V, 0, 0, 0 },
  { 5, 5, 7, 4, 7, 1, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_L, 0, 0, 0 },
  { 5, 5, 7, 4, 7, 2, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_W, 0, 0, 0 },
  { 5, 5, 7, 4, 7, 3, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_X, 0, 0, 0 },
  { 5, 5, 7, 4, 7, 4, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_Q, 0, 0, 0 },
  { 5, 5, 7, 4, 7, 5, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_C, 0, 0, 0 },
  { 5, 5, 7, 4, 7, 6, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_F, 0, 0, 0 },
  { 5, 5, 7, 4, 7, 7, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_B, 0, 0, 0 },
  { 5, 5, 7, 4, 7, 8, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_G, 0, 0, 0 },
  { 5, 5, 7, 4, 7, 9, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_Z, 0, 0, 0 },
  { 5, 5, 7, 4, 8, 0, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_BACK_SLASH, 0, 0, 0 },
  { 5, 5, 7, 4, 8, 1, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_J, 0, 0, 0 },
  { 5, 5, 7, 4, 8, 2, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_S, 0, 0, 0 },
  { 5, 5, 7, 4, 8, 3, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET2, VK_S, 0, 0, 0 },
  { 5, 5, 7, 4, 8, 4, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_H, 0, 0, 0 },
  { 5, 5, 7, 4, 8, 5, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET2, VK_J, 0, 0, 0 },
  { 5, 5, 7, 4, 8, 6, 0 },
  /* Ligature { + character */

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_T, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 4, 8, 7, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_K, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 4, 8, 8, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_R, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 4, 8, 9, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_M, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 4, 9, 0, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_N, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 4, 9, 1, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_P, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 4, 9, 2, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_D, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 4, 9, 3, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_V, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 4, 9, 4, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_L, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 4, 9, 5, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_W, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 4, 9, 6, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_X, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 4, 9, 7, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_Q, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 4, 9, 8, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_C, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 4, 9, 9, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_F, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 0, 0, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_B, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 0, 1, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_G, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 0, 2, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_Z, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 0, 3, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_BACK_SLASH, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 0, 4, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_J, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 0, 5, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_S, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 0, 6, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET2, VK_S, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 0, 7, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_H, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 0, 8, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET2, VK_J, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 0, 9, 0 },
  /* Ligature [ + character + /   */


  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_T, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 1, 0, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_K, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 1, 1, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_R, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 1, 2, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_M, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 1, 3, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_N, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 1, 4, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_P, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 1, 5, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_D, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 1, 6, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_V, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 1, 7, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_L, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 1, 8, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_W, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 1, 9, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_X, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 2, 0, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_Q, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 2, 1, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_C, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 2, 2, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_F, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 2, 3, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_B, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 2, 4, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_G, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 2, 5, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_Z, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 2, 6, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_BACK_SLASH, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 2, 7, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_J, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 2, 8, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_S, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 2, 9, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET2, VK_S, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 3, 0, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET1, VK_H, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 3, 1, 0 },

  { BUCKET2, VK_BRA_OPEN, BUCKET2, VK_J, BUCKET1, VK_FOR_SLASH, 0 },
  { 5, 5, 7, 5, 3, 2, 0 },
  /* Ligature { + character + /   */


  { BUCKET1, VK_T, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 3, 3, 0 },

  { BUCKET1, VK_K, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 3, 4, 0 },

  { BUCKET1, VK_R, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 3, 5, 0 },

  { BUCKET1, VK_M, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 3, 6, 0 },

  { BUCKET1, VK_N, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 3, 7, 0 },

  { BUCKET1, VK_P, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 3, 8, 0 },

  { BUCKET1, VK_D, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 3, 9, 0 },

  { BUCKET1, VK_V, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 4, 0, 0 },

  { BUCKET1, VK_L, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 4, 1, 0 },

  { BUCKET1, VK_W, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 4, 2, 0 },

  { BUCKET1, VK_X, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 4, 3, 0 },

  { BUCKET1, VK_Q, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 4, 4, 0 },

  { BUCKET1, VK_C, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 4, 5, 0 },

  { BUCKET1, VK_F, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 4, 6, 0 },

  { BUCKET1, VK_B, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 4, 7, 0 },

  { BUCKET1, VK_G, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 4, 8, 0 },

  { BUCKET1, VK_Z, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 4, 9, 0 },

  { BUCKET1, VK_BACK_SLASH, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 5, 0, 0 },

  { BUCKET1, VK_J, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 5, 1, 0 },

  { BUCKET1, VK_S, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 5, 2, 0 },

  { BUCKET2, VK_S, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 5, 3, 0 },

  { BUCKET1, VK_H, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 5, 4, 0 },

  { BUCKET2, VK_J, BUCKET2, VK_BRA_CLOSE, 0, 0, 0 },
  { 5, 5, 7, 5, 5, 5, 0 },
  /* Ligature character + } */


  { BUCKET1, VK_J, BUCKET3, VK_H, 0, 0, 0 },
  { 5, 5, 7, 5, 5, 6, 0 },

  { BUCKET1, VK_S, BUCKET3, VK_H, 0, 0, 0 },
  { 5, 5, 7, 5, 5, 7, 0 },

  { BUCKET2, VK_S, BUCKET3, VK_H, 0, 0, 0 },
  { 5, 5, 7, 5, 5, 8, 0 },

  { BUCKET1, VK_H, BUCKET3, VK_H, 0, 0, 0 },
  { 5, 5, 7, 5, 5, 9, 0 },

  { BUCKET2, VK_J, BUCKET3, VK_H, 0, 0, 0 },
  { 5, 5, 7, 5, 6, 0, 0 },
  /* Ligature character + CA h */


  { BUCKET1, VK_O, BUCKET1, VK_F, 0, 0, 0 },
  { 5, 5, 7, 5, 6, 1, 0 },
  /* Ligature character + f */


  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_T, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 6, 2, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_K, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 6, 3, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_R, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 6, 4, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_M, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 6, 5, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_N, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 6, 6, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_P, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 6, 7, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_D, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 6, 8, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_V, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 6, 9, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_L, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 7, 0, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_W, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 7, 1, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_X, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 7, 2, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_Q, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 7, 3, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_C, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 7, 4, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_F, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 7, 5, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_B, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 7, 6, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_G, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 7, 7, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_Z, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 7, 8, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_BACK_SLASH, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 7, 9, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_J, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 8, 0, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_S, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 8, 1, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET2, VK_S, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 8, 2, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET1, VK_H, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 8, 3, 0 },

  { BUCKET1, VK_BRA_OPEN, BUCKET2, VK_J, BUCKET1, VK_F, 0 },
  { 5, 5, 7, 5, 8, 4, 0 },
  /* Ligature [ + character + f   */


  { 0, 0, 0, 0, 0, 0, 0 } // Terminating last row.
};
