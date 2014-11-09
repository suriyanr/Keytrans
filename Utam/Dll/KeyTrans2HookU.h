// Function prototypes.

void log_string();
void DebugWriteMap();
const char *VKCodeToString(unsigned char VKCode);
unsigned char VKStringToCode(char *VKString);
const char *BucketCodeToString(unsigned char BucketCode);
unsigned char BucketStringToCode(char *BucketString);
int InitialiseMap(char *MapFile);
unsigned char VKNumPadToScanCode(unsigned char vk);
int TranslateLHS(unsigned int LHS[], unsigned int RHS[7]);

#define VK_PACKET        0xE7

#define VK_CTRL          0x11
#define VK_ALT           0x12
#define VK_CAPSLOCK      0x14
#define VK_BACKSPACE     0x08
#define SCAN_CTRL        0x1D
#define SCAN_ALT         0x38
#define SCAN_BACKSPACE   0x0E

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
#define VK_NUMPAD_PLUS 0x6B // So its 0xb in the translation table.


// Below is all the Virtual Keys that we are interested in.
// We have four rows of keys as below:
// First  ROW: `1234567890-=
// Second ROW: qwertyuiop[]\
// Third  ROW: asdfghjkl;'
// Fourth ROW: zxcvbnm,./
// And the space bar.

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

// The Space bar - already defined in windows.h
// #define VK_SPACE      0x20
// The Tab - already defined in windows.h
// #define VK_TAB        0x09

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
#define STR_VK_B          "VK_B"
#define STR_VK_N          "VK_N"
#define STR_VK_M          "VK_M"
#define STR_VK_COMMA      "VK_COMMA"
#define STR_VK_DOT        "VK_DOT"
#define STR_VK_FOR_SLASH  "VK_FOR_SLASH"

// The Space bar
#define STR_VK_SPACE      "VK_SPACE"
#define STR_VK_TAB        "VK_TAB"

#define STR_VK_NULL       ""


#define STR_BUCKET1       "BUCKET1"
#define STR_BUCKET2       "BUCKET2"
#define STR_BUCKET3       "BUCKET3"
#define STR_BUCKET4       "BUCKET4"
#define BUCKET1           1
#define BUCKET2           2
#define BUCKET3           3
#define BUCKET4           4


