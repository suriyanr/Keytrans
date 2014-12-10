#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <winable.h>

#ifndef KEYEVENTF_UNICODE
# define KEYEVENTF_UNICODE 0x00000004
#endif

#include "KeyTrans2HookU.h" 


#include <stdio.h>

#if 0 // Turn it to 1 to turn debug on.
#define DEBUG 1
#endif

HINSTANCE hinst; 

// Below are the variables shared between the exe and the dll
// Note that on searching the net the #pragma directives are given
// as examples to make them shared across exe's and dll's but they
// don't work with mingw, even though there is no warning or error
// generated if used.

#ifdef DEBUG
char __attribute__((section ("SHARED"),shared)) DebugFile[512] = { 0 };
#endif

// The variable that tells if the user has enabled or disabled the 
// translation.
int __attribute__((section("SHARED"),shared)) HookTranslationEnabled = 0;

// The global key state statics.
int __attribute__((section ("SHARED"),shared)) CapsOn = 0;
int __attribute__((section ("SHARED"),shared)) ShiftOn = 0;
int __attribute__((section ("SHARED"),shared)) CtrlOn = 0;
int __attribute__((section ("SHARED"),shared)) AltOn = 0;
int __attribute__((section ("SHARED"),shared)) AltOnR = 0;

HHOOK __attribute__((section ("SHARED"),shared)) hhk = NULL;

// The translator array populated by the map file.
// Each row has 15 integers. What they stand for is as below:
// Number of strokes, bucket, stroke, bucket, stroke, bucket, stroke
//    bucket, stroke. This makes 9 integers. stroke stands for the
//    scan code of the key pressed. bucket represents the bucket the
//    key stroke represents. Number of strokes stands for how many
//    stroke pairs are represented in this row for the input (LHS) of
//    the rule.
//  The above 9 integers dictate the LHS of the rule mapping.
// The remainder 6 integers represent what we are translating the keystroke
// into. Its format is as follows:
// Number of characters, unicode of character 1, unicode of character 2,
//  unicode of character 3, unicode of character 4, unicode of character 5.
// Number of characters tells us how many unicode characters we are typing in.
// The rest of them specify the unicode of the characters to be typed in.
// Below define is present in KeyTransU.c too.
#define MAXMAPENTRIES 1000
unsigned int __attribute__((section ("SHARED"),shared)) KeyTrans[MAXMAPENTRIES][15]={0};

// Integer for reading the Scan codes. Same order in which the
// StrArray lists the STR_VK strings
unsigned char MapScanArray[] = {
   VK_APOSTROPHE,
   VK_0,
   VK_1,
   VK_2,
   VK_3,
   VK_4,
   VK_5,
   VK_6,
   VK_7,
   VK_8,
   VK_9,
   VK_MINUS,
   VK_EQUAL,

   VK_Q,
   VK_W,
   VK_E,
   VK_R,
   VK_T,
   VK_Y,
   VK_U,
   VK_I,
   VK_O,
   VK_P,
   VK_BRA_OPEN,
   VK_BRA_CLOSE,
   VK_BACK_SLASH,

   VK_A,
   VK_S,
   VK_D,
   VK_F,
   VK_G,
   VK_H,
   VK_J,
   VK_K,
   VK_L,
   VK_COLON,
   VK_QUOTES,

   VK_Z,
   VK_X,
   VK_C,
   VK_V,
   VK_B,
   VK_N,
   VK_M,
   VK_COMMA,
   VK_DOT,
   VK_FOR_SLASH,

   VK_SPACE,
   VK_TAB,

   VK_NULL
};

// String array for reading the map file translations.
char *MapStrArray[] = {
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

   STR_VK_SPACE,
   STR_VK_TAB,

   STR_VK_NULL
};

// End of the shared variables.


// History of four last characters.
// Each character takes two slots.
// First slot = BUCKET (BUCKET1|BUCKET2|BUCKET3|BUCKET4)
// Second slot = VK_Code
// The most recent is in index 6 and 7.
// Oldest is in index 0, 1.
// Note that this is global per process, so that each process has its own
// KeysHistory[]
unsigned int KeysHistory[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

// This holds the history of the number of characters that each of the past
// keystrokes translated out to be. This helps in determining how many 
// Backspaces to send for the current translation.
// KeysHistoryTranslation[0][] holds the counts of the RHS
// KeysHistoryTranslation[1][] holds the counts of the LHS
unsigned int KeysHistoryTranslation[2][4] = { {0, 0, 0, 0 },
                                              {0, 0, 0, 0 }
                                            };

#ifdef DEBUG
char str_debug[512];

void SetDebugFile(char *filename) {
FILE *log_file;

   strcpy(DebugFile, filename);

   // Lets create the file as well.
   log_file = fopen(DebugFile, "w");
   fclose(log_file);
}

void log_string(char *str_debug) {
FILE *log_file;

   // Lets log it.
   log_file = fopen(DebugFile, "a+");
   fprintf(log_file, "%s", str_debug);
   fclose(log_file);
}

// Writes out the mapping read from the map file in the debug log.
void DebugWriteMap() {
int Index;
char temp_str[1024];
int bucket_loop;
const char *str1, *str2;

   sprintf(str_debug, "Start - Dumping the KeyTrans[] array\n");
   log_string(str_debug);

   // Lets print out the KeyTrans[][] array in a readable form so we can
   // check if we read the map file right.
   // Input keystrokes[count]: blah blah | Output characters[count]: numbers
   // The above seems to be good enough.
   Index = 0;
   while (KeyTrans[Index][0] != 0) {
      sprintf(str_debug, "Rule: %d Input Keystrokes[%d]: ", Index, KeyTrans[Index][0]);
      for (bucket_loop = 1; bucket_loop <= KeyTrans[Index][0]; bucket_loop++) {
         str1 = BucketCodeToString(KeyTrans[Index][2 * bucket_loop - 1]);
         str2 = VKCodeToString(KeyTrans[Index][2 * bucket_loop]);
         sprintf(temp_str, "%s %s ", str1, str2);
         strcat(str_debug, temp_str);
      }
      // Continue on to RHS ...
      // For RHS [][9] has has the count of unicode characters.
      sprintf(temp_str, "| Output characters[%d]: ", KeyTrans[Index][9]);
      strcat(str_debug, temp_str);
      for (bucket_loop = 1; bucket_loop <= KeyTrans[Index][9]; bucket_loop++) {
         sprintf(temp_str, "%d ", KeyTrans[Index][bucket_loop + 9]);
         strcat(str_debug, temp_str);
      }
      strcat(str_debug, "\n");
      log_string(str_debug);

      Index++;
   }

   sprintf(str_debug, "End - Dumping the KeyTrans[] array\n");
   log_string(str_debug);
}
#endif

// Function which return the VK String when passed the VK code.
// Array MapStrArray[] has a list of the VK Strins.
// Array MapScanArray[] has a list of the VK Scan codes.
// Both arrays are ordered the same way. So we we get the index from the Scan
// array and return the value from the Str array.
const char *VKCodeToString(unsigned char VKCode) {
const char *VKString = NULL;
int index;

   index = 0;
   while (1) {
      if (MapScanArray[index] == 0) break;

      if (MapScanArray[index] == VKCode) {
         VKString = MapStrArray[index];
         break;
      }
      index++;
   }
   return(VKString);
}


// Function which returns the VK code when passed the VK String.
// Array MapStrArray[] has a list of the VK Strins.
// Array MapScanArray[] has a list of the VK Scan codes.
// Both arrays are ordered the same way. So we we get the index from the Str 
// array and return the value from the Scan array.
unsigned char VKStringToCode(char *VKString) {
unsigned char vk_key = 0;
int index;

   index = 0;
   while (1) {
      if (MapStrArray[index][0] == '\0') break;

      if (strcasecmp(VKString, MapStrArray[index]) == 0) {
         vk_key = MapScanArray[index];
         break;
      }
      index++;
   }
   return(vk_key);
}

// Function which returns the Bucket String when passed the Bucket code.
const char *BucketCodeToString(unsigned char BucketCode) {
const char *bucket_str = NULL;

   if (BucketCode == BUCKET1) {
      bucket_str = STR_BUCKET1;
   } 
   else if (BucketCode == BUCKET2) {
      bucket_str = STR_BUCKET2;
   }
   else if (BucketCode == BUCKET3) {
      bucket_str = STR_BUCKET3;
   }
   else if (BucketCode == BUCKET4) {
      bucket_str = STR_BUCKET4;
   }
   return(bucket_str);
}


// Function which returns the Bucket code when passed the Bucket String.
unsigned char BucketStringToCode(char *BucketString) {
unsigned char bucket_code = 0;

   if (strcasecmp(STR_BUCKET1, BucketString) == 0) {
      bucket_code = BUCKET1;
   }
   else if (strcasecmp(STR_BUCKET2, BucketString) == 0) {
      bucket_code = BUCKET2;
   }
   else if (strcasecmp(STR_BUCKET3, BucketString) == 0) {
      bucket_code = BUCKET3;
   }
   else if (strcasecmp(STR_BUCKET4, BucketString) == 0) {
      bucket_code = BUCKET4;
   }
   return(bucket_code);
}

// Read into KeyTrans[][] array the map from the map file.
// The Name of the file is passed as the first parameter.
// On success returns 0.
// On failure it returns a positive integer which is the line
// number in the map file which it couldnt process meaningfully.
// -1 => Error opening the map file.
// -2 => Number of mappings exceeds MAXMAPENTRIES
int InitialiseMap(char *MapFile) {
int RetCode = -1;
FILE *fp;
// max 8 fields on the LHS
// 9th field is the = sign
// max 5 integers on the RHS
// Total 14 fields max.
char Field[14][64];
char InputField[1024];
int LHSCount;
int RHSCount;
int RHSIndex;
int KeyTransIndex = 0;
unsigned char BucketCode;
unsigned char VKCode;
unsigned int UnicodeInt;
int TempNum;

   // First zero out the entire KeyTrans[][] array.
   // We do this as we could be loading more than one map file.
#ifdef DEBUG
   sprintf(str_debug, "Size of KeyTrans array is: %d\n", sizeof(KeyTrans));
   log_string(str_debug);
#endif
   memset(KeyTrans, 0, sizeof(KeyTrans));

   fp = fopen(MapFile, "r");
   if (fp == NULL) return(RetCode);

#ifdef DEBUG
   sprintf(str_debug, "Succesfully opened MapFile:  %s\n", MapFile);
   log_string(str_debug);
#endif

   // We have a valid fp.
   RetCode = 0;
   while (fgets(InputField, sizeof(InputField), fp)) {
      // Lets initialize Field[][] so its sane
      for (TempNum = 0; TempNum < 14; TempNum++) Field[TempNum][0] = '\0';

      // Update Retcode to have the current line number
      RetCode++;

      // We have read in a line in InputField. Lets process it.
      sscanf(InputField, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                         Field[0], Field[1], Field[2], Field[3], Field[4],
                         Field[5], Field[6], Field[7], Field[8], Field[9],
                         Field[10], Field[11], Field[12], Field[13]);
#ifdef DEBUG
      sprintf(str_debug, "Processing Line: %s\n", InputField);
      log_string(str_debug);
      sprintf(str_debug, "Field[0]: %s Field[1]: %s Field[2]: %s Field[3]: %s Field[4]: %s Field[5]: %s Field[6]: %s Field[7]: %s", Field[0], Field[1], Field[2], Field[3], Field[4], Field[5], Field[6], Field[7]);
      log_string(str_debug);
      sprintf(str_debug, " Field[8]: %s Field[9]: %s Field[10]: %s Field[11]: %s Field[12]: %s Field[13]: %s\n", Field[8], Field[9], Field[10], Field[11], Field[12], Field[13]);
      log_string(str_debug);

#endif

      // Lets skip over lines which start with a #
      if (Field[0][0] == '#') {
#ifdef DEBUG
          sprintf(str_debug, "Skipping comment line\n");
          log_string(str_debug);
#endif
          continue;
      }

      // Lets skip over lines which are empty too
      if (Field[0][0] == '\0') {
#ifdef DEBUG
          sprintf(str_debug, "Skipping empty line\n");
          log_string(str_debug);
#endif
          continue;
      }

      // Lets process the line.
      // Field[0] has to be one of the BUCKETs.
      // Field[1] has to be one of the VK_ strings.
      // Lets loop through and read the LHS out.
      // Max 4 pairs of BUCKET/VK_STRING.
      LHSCount = 0;
      while (1) {
          if (LHSCount > 4) {
             // We exceeded the max of 4 BUCKET/STRING pairs for the LHS
             fclose(fp);
             return(RetCode);
          }
          BucketCode = BucketStringToCode(Field[LHSCount * 2]);
#ifdef DEBUG
          sprintf(str_debug, "Bucket String: %s Bucket Code: %d\n", Field[LHSCount * 2], BucketCode);
          log_string(str_debug);
#endif

          // If its not a BUCKET string and we have already read some LHS
          // words, and we hit the '=' string, lets move on.
          if ( (BucketCode == 0) &&
               LHSCount &&
               (strcmp(Field[LHSCount * 2], "=") ==0)
             ) {
             RHSIndex = LHSCount * 2 + 1;
             KeyTrans[KeyTransIndex][0] = LHSCount;
#ifdef DEBUG
             sprintf(str_debug, "Hit = sign. LHS Count: %d RHSIndex: %d. Moving on to RHS processing\n", LHSCount + 1, RHSIndex);
             log_string(str_debug);
#endif
             break;
          }

          if (BucketCode == 0) {
#ifdef DEBUG
             sprintf(str_debug, "BucketCode is 0\n");
             log_string(str_debug);
#endif
             fclose(fp);
             return(RetCode);
          }
          VKCode = VKStringToCode(Field[LHSCount * 2 + 1]);
          if (VKCode == VK_NULL) {
#ifdef DEBUG
             sprintf(str_debug, "VKCode is 0\n");
             log_string(str_debug);
#endif
             fclose(fp);
             return(RetCode);
          }

          // We got the BucketCode and the VKCode
          // First check if KeyTransIndex is in a valid range
          if (KeyTransIndex >= MAXMAPENTRIES) {
#ifdef DEBUG
             sprintf(str_debug, "Number of map entries exceeds %d\n", MAXMAPENTRIES);
             log_string(str_debug);
#endif
             fclose(fp);
             return(-2);
          }
          KeyTrans[KeyTransIndex][LHSCount * 2 + 1] = BucketCode;
          KeyTrans[KeyTransIndex][LHSCount * 2 + 2] = VKCode;
#ifdef DEBUG
          sprintf(str_debug, "Translated: BucketCode: %d VKCode: %d for input string: %s %s\n", BucketCode, VKCode, Field[LHSCount * 2], Field[LHSCount * 2 + 1]);
          log_string(str_debug);
#endif
          LHSCount++;
      }

      // We are here => we have successfully read the LHS
      // and got the '=' sign. RHSIndex now points to the start
      // of the RHS parameters in Field[]
      RHSCount = 0;
      while (1) {
         char *EndPtr;

#ifdef DEBUG
         sprintf(str_debug, "Field[%d] : %c in dec: %d\n", RHSIndex, Field[RHSIndex][0], Field[RHSIndex][0]);
         log_string(str_debug);
#endif

         // If we have at least got one field for the RHS
         // and we have no more numbers, we are done.
         if ( (RHSCount) &&
              (Field[RHSIndex][0] == '\0')
            ) {
#ifdef DEBUG
            sprintf(str_debug, "Done with RHS processing. Moving to next line.\n");
            log_string(str_debug);
#endif

            KeyTrans[KeyTransIndex][9] = RHSCount;
            break;
         }

         if (RHSCount == 5) {
#ifdef DEBUG
            sprintf(str_debug, "Exceeded max of 5 unicode characters for the RHS.\n");
            log_string(str_debug);
#endif

            // We exceeded the max of 5 unicode characters for the RHS
            fclose(fp);
            return(RetCode);
         }

         // We use 0 as the base so standard C conventions can be used to input
         // numbers in the map file. 0xFF for hex etc.
         UnicodeInt = strtoul(Field[RHSIndex], &EndPtr, 0);

#ifdef DEBUG
         sprintf(str_debug, "RHS[%d] = %d\n", RHSCount, UnicodeInt);
         log_string(str_debug);
#endif

         if (*EndPtr != '\0') {
            // Invalid input
#ifdef DEBUG
            sprintf(str_debug, "*EndPtr not equal to null character hence invalid input\n");
            log_string(str_debug);
#endif
            fclose(fp);
            return(RetCode);
         }

         KeyTrans[KeyTransIndex][10 + RHSCount] = UnicodeInt;

         RHSIndex++;
         RHSCount++;
      }

      // We are done processing a row in KeyTrans[][]
      // Lets print out the processed array.
#ifdef DEBUG
      for (TempNum = 0; TempNum < 15; TempNum++) {
         sprintf(str_debug, "KeyTrans[%d][%d]: %d ", KeyTransIndex, TempNum, KeyTrans[KeyTransIndex][TempNum]);
         log_string(str_debug);
      }
      sprintf(str_debug, "\n");
      log_string(str_debug);
#endif
      
      KeyTransIndex++;
   }

   // Success reading in the map file.
   fclose(fp);
   return(0);
}

// In this version we translate if possible on all key presses,
// depending on what bucket we currently are on.
// Normal = Bucket 1 translations.
// CapsLock On and Shift not pressed or CapsLock off and Shift pressed
//   => Bucket 2 translations.
// Alt pressed => Bucket 3 translations.
// Ctrl and Alt pressed => Bucket 4 Translations.
static LRESULT CALLBACK KHookProc(int code, WPARAM wParam, LPARAM lParam)
{
int extended;
int keyrelease;
unsigned char vk;

   // if code < 0 just do normal stuff.
   if (code < 0) {
      return CallNextHookEx(hhk, code, wParam, lParam);
   }

   // Lets just handle the HC_ACTION case = 0 
   // HC_ACTION 0; The wParam and lParam contain info about a keystroke
   //              message
   // HC_NOREMOVE 3; The wParam and lParam contain information about a 
   //                a keystroke message and it has not been removed
   //                from the message queue.
   if (code != HC_ACTION) {
      return CallNextHookEx(hhk, code, wParam, lParam);
   }

//   repeatcount = lParam & 0x0ffff;
//   scancode    = (lParam & 0xff0000) >> 16;
   extended    = (lParam & 0x1000000) >> 24;
//   alt         = (lParam & 0x20000000) >> 29;
//   prestate    = (lParam & 0x40000000) >> 30;
   keyrelease  = (lParam & 0x80000000) >> 31;

   vk = wParam;

#if 0
   sprintf(str_debug, "Entering hook, vk = %x, code = %x, lparam = %x\n", vk, code, lParam);
   log_string(str_debug);
#endif

   // If its VK_BACKSPACE or any of the numpad keys, or a Unicode packet = 
   // VK_PACKET, we dont process it.
   if ( (vk == VK_PACKET) ||
        (vk == VK_BACKSPACE) ||
        ( (vk >= VK_NUMPAD0) && (vk <= VK_NUMPAD9) )
      ) {
#ifdef DEBUG
      if (vk == VK_PACKET) {
         sprintf(str_debug, "Calling next hook as its Unicode. lParam: %x\n", (lParam & 0xFF000000) >> 24);
         log_string(str_debug);
      }
      else if (vk == VK_BACKSPACE) {
         sprintf(str_debug, "Calling next hook as its Backspace.\n");
         log_string(str_debug);
      }
      else if ( (vk >= VK_NUMPAD0) && (vk <= VK_NUMPAD9) ) {
         sprintf(str_debug, "Calling next hook as its the num pad keys.\n");
         log_string(str_debug);
      }
#endif

      return CallNextHookEx(hhk, code, wParam, lParam);
   }

   // Remember state of Caps lock key.
   if (vk == VK_CAPITAL) {
      if (keyrelease == 1) {
         if (CapsOn == 1) CapsOn = 0;
         else CapsOn = 1;
#ifdef DEBUG
         sprintf(str_debug, "CapsOn noted as %d\n", CapsOn);
         log_string(str_debug);
#endif
      }
      // VK_CAPITAL is pressed we do not want to log this key, so we are done.
      return CallNextHookEx(hhk, code, wParam, lParam);
   }

   // Remember state of Ctrl Key.
   if ( (vk == VK_CONTROL) || (vk == VK_LCONTROL) || (vk == VK_RCONTROL) ) {
      if (keyrelease == 1) {
         CtrlOn = 0;
      }
      else {
         CtrlOn = 1;
      }
#ifdef DEBUG
      sprintf(str_debug, "Ctrl noted as %d\n", CtrlOn);
      log_string(str_debug);
#endif

      // VK_?CONTROL is pressed we do not want to log this key, so we are done.
      return CallNextHookEx(hhk, code, wParam, lParam);
   }

   // Remember state of Shift Key.
   if ( (vk == VK_SHIFT) || (vk == VK_LSHIFT) || (vk == VK_RSHIFT) ) {
      if (keyrelease == 1) {
         ShiftOn = 0;
      }
      else {
         ShiftOn = 1;
      }
#ifdef DEBUG
      sprintf(str_debug, "Shift noted as %d\n", ShiftOn);
      log_string(str_debug);
#endif

      // VK_?SHIFT is pressed we do not want to log this key, so we are done.
      return CallNextHookEx(hhk, code, wParam, lParam);
   }

   // Remember state of Alt Key.
   if ( (vk == VK_MENU) || (vk == VK_LMENU) || (vk == VK_RMENU) ) {
      if (keyrelease == 1) {
         AltOn = 0;
         AltOnR = 0;
      }
      else {
         AltOn = 1;
         // Remember if Right Alt was pressed.
         if (extended)
           AltOnR = 1;
      }
#ifdef DEBUG
      sprintf(str_debug, "Alt noted as %d Extended: %d\n", AltOn, AltOnR);
      log_string(str_debug);
#endif

      // VK_?MENU is pressed we do not want to log this key, so we are done.
      return CallNextHookEx(hhk, code, wParam, lParam);
   }


   // Handle key presses
   if (keyrelease == 0) {
   int i, count;
   unsigned char bucket;
   unsigned int RHS[8];
   unsigned int KH[9];
   INPUT   Input[8];
   UINT    SendInputRetVal;

#if 0
      // Lets check if Alt-0 to disable the hook translation code or
      // Alt-1 to enabled the hook translation code.
      if ( (AltOn == 1) && (ShiftOn == 0) && (CtrlOn == 0) &&
           (vk == VK_0) ) {
         // Key press by user to disable hook translation.
         HookTranslationEnabled = 0;
         return CallNextHookEx(hhk, code, wParam, lParam);
      }
      if ( (AltOn = 1) && (ShiftOn == 0) && (CtrlOn == 0) &&
           (vk == VK_1) ) {
         // Key press by user to enable hook translation.
         HookTranslationEnabled = 1;
         return CallNextHookEx(hhk, code, wParam, lParam);
      }
#endif


// NEW CODE CTRL-SPACE to enable/disable the hook.
      // Lets check if CTRL->SPACE to enable/disable (toggle) the hook 
      // translation code
      if ( (AltOn == 0) && (ShiftOn == 0) && (CtrlOn == 1) &&
           (vk == VK_SPACE) ) {
#ifdef DEBUG
           sprintf(str_debug, "Ctrl-Space has been hit\n");
           log_string(str_debug);
#endif
         if (HookTranslationEnabled == 1) {
            HookTranslationEnabled = 0;
         }
         else {
            HookTranslationEnabled = 1;
         }
#ifdef DEBUG
            sprintf(str_debug, "HookTranslationEnabled set to %x\n", HookTranslationEnabled);
            log_string(str_debug);
#endif
         // We eat the CTRL-SPACE keystroke.
         return -1;
      }
// END of NEW CODE


      // Just return out if hook translation is disabled.
      if (HookTranslationEnabled == 0) {
         return CallNextHookEx(hhk, code, wParam, lParam);
      }

      // We need to do nothing if a key is pressed and the following is true
      // (CtrlOn == 1) && (AltOn == 0) (doesnt matter shift or caps)
      // (CtrlOn == 1) && (AltOn == 1) && ((ShiftOn == 1) || (CapsOn == 1))
      if ((CtrlOn == 1) && (AltOn == 0)) {
#ifdef DEBUG
           sprintf(str_debug, "Ctrl and no Alt - no bucket\n");
           log_string(str_debug);
#endif
         return CallNextHookEx(hhk, code, wParam, lParam);
      }
      if ((CtrlOn == 1) && (AltOn == 1) && ((ShiftOn == 1) || (CapsOn == 1))) {
#ifdef DEBUG
           sprintf(str_debug, "Ctrl and Alt and Caps/Shift - no bucket\n");
           log_string(str_debug);
#endif
         return CallNextHookEx(hhk, code, wParam, lParam);
      }

      // Lets get the bucket.
      // If Capslock is off and shift not pressed = bucket1
      // If Capslock is on and shift not pressed = bucket2
      // If Capslock is on and shift is pressed = bucket1
      // If Capslock is off and shift is pressed = bucket2
      // If Alt pressed = bucket3.
      // Ctrl + Alt = bucket 4.

      // Initialise bucket to BUCKET1
      bucket = BUCKET1;
#ifdef DEBUG
      sprintf(str_debug, "bucket is BUCKET1\n");
      log_string(str_debug);
#endif

      // Lets take care of the Shift and Caps variation for Bucket two
      if ( (CtrlOn == 0) && (AltOn == 0) ) {
         if ( ((CapsOn == 0) && (ShiftOn == 1)) ||
              ((CapsOn == 1) && (ShiftOn == 0)) ) {
            bucket = BUCKET2;
#ifdef DEBUG
            sprintf(str_debug, "bucket is BUCKET2\n");
            log_string(str_debug);
#endif
         }
      }

// Lets try Alt = BUCKET3
#if 0
      // With above Bucket 1 and Bucket 2 is determined.
      // Bucket 3 is Ctrl
      if ( (AltOn == 0) && (CtrlOn == 1) && (CapsOn == 0) && (ShiftOn == 0) ) {
         bucket = BUCKET3;
      }
#else
      // With above Bucket 1 and Bucket 2 is determined.
      // Bucket 3 is Alt
      if ( (AltOn == 1) && (CtrlOn == 0) && (CapsOn == 0) && (ShiftOn == 0) ) {
         bucket = BUCKET3;
#ifdef DEBUG
         sprintf(str_debug, "bucket is BUCKET3\n");
         log_string(str_debug);
#endif
      }

#endif

      // What we have pending is Bucket 4.
      if ( (AltOn == 1) && (CtrlOn == 1) && (CapsOn == 0) && (ShiftOn == 0) ) {
         bucket = BUCKET4;
#ifdef DEBUG
         sprintf(str_debug, "bucket is BUCKET4\n");
         log_string(str_debug);
#endif
      }

      // Lets log the key.
      // Move index 2, 3, 4, 5, 6, 7 onto index 0, 1, 2, 3, 4, 5
      for (i = 0; i < 6; i++) {
         KeysHistory[i] = KeysHistory[i + 2];
      }

      // Move index 1, 2, 3 onto index 0, 1, 2.
      for (i = 0; i < 3; i++) {
         KeysHistoryTranslation[0][i] = KeysHistoryTranslation[0][i + 1];
         KeysHistoryTranslation[1][i] = KeysHistoryTranslation[1][i + 1];
      }

      KeysHistory[6] = bucket;
      KeysHistory[7] = vk;
      KeysHistoryTranslation[0][3] = 1;
      KeysHistoryTranslation[1][3] = 1;

#ifdef DEBUG
      sprintf(str_debug, "Input vk: %d\n", vk);
      log_string(str_debug);
      sprintf(str_debug, "KeysHistory: %s %s ", BucketCodeToString(KeysHistory[0]), VKCodeToString(KeysHistory[1]));
      log_string(str_debug);
      sprintf(str_debug, "%s %s ", BucketCodeToString(KeysHistory[2]), VKCodeToString(KeysHistory[3]));
      log_string(str_debug);
      sprintf(str_debug, "%s %s ", BucketCodeToString(KeysHistory[4]), VKCodeToString(KeysHistory[5]));
      log_string(str_debug);
      sprintf(str_debug, "%s %s\n", BucketCodeToString(KeysHistory[6]), VKCodeToString(KeysHistory[7]));
      log_string(str_debug);
#endif

      // Now we check if it matches a LHS rule.
      // We eat the key if it matches the LHS Rule, as we
      // will inject the corresponding RHS Rule.
      // First check if the longest LHS Rule matches. (4 keystrokes)

      KH[0] = 4;
      memcpy(&KH[1], KeysHistory, sizeof(unsigned int) * 8);
      count = TranslateLHS(KH, RHS);
      if (count == 0) {
         // Now check if the shorter LHS Rule matches. (3 keystrokes)
         KH[0] = 3;
         memcpy(&KH[1], &KeysHistory[2], sizeof(unsigned int) * 6);
         count = TranslateLHS(KH, RHS);
      }
      if (count == 0) {
         // Now check if the shorter LHS Rule matches. (2 keystrokes)
         KH[0] = 2;
         memcpy(&KH[1], &KeysHistory[4], sizeof(unsigned int) * 4);
         count = TranslateLHS(KH, RHS);
      }
      if (count == 0) {
         // Now check if the shortest LHS Rule matches. (1 keystroke)
         KH[0] = 1;
         memcpy(&KH[1], &KeysHistory[6], sizeof(unsigned int) * 2);
         count = TranslateLHS(KH, RHS);
      }
      if (count != 0) {
#ifdef DEBUG
         sprintf(str_debug, "KeyRelease: Translation spotted: count: %d\n", count);
         log_string(str_debug);
#endif
         // Update the Translation count.
         KeysHistoryTranslation[0][3] = count;
         KeysHistoryTranslation[1][3] = KH[0];

         // How many BS's do we send?
         // The matching number of key strokes is KH[0] for which
         // this translation is being acted upon.
         // So we look at the last KH[0] - 1 history translations.
         // + KH[0] - 1
         // So if KH[0] = 3.
         // We add KeysHistoryTranslation[0][2] + KeysHistoryTranslation[0][1]
         // We need backspaces only when there has been a ligature
         // expansion of at least 2 characters in the history.
         if (KH[0] > 1) {
         int BS_count = 0;
         int BS_KHTindex;

            // Check if this ligature sequence length is less than or
            // equal to the previous ligature sequence length.
            // index 3 is current, 2 is previous and so on.
            if (KeysHistoryTranslation[1][3] <= KeysHistoryTranslation[1][2]) {
               // This is where we find what could have been the possible
               // count for the previous ligature.
               unsigned int C_RHS[8]; // C_ added to mean for correction.
               unsigned int C_KH[9];
               C_KH[0] = KeysHistoryTranslation[1][3] - 1;

               // Lets populate C_KH[] from KeysHistory[]
               memcpy(&C_KH[1], &KeysHistory[2], sizeof(unsigned int) * 2 * C_KH[0]);
               BS_count = TranslateLHS(C_KH, C_RHS);

#ifdef DEBUG
               sprintf(str_debug, "Exception case of BS_count generation. current liga sequent: %d previous liga seq: %d BS_count: %d\n", KeysHistoryTranslation[1][3], KeysHistoryTranslation[1][2], BS_count);
               log_string(str_debug);
#endif
            }
            else {

               BS_KHTindex = 3 - (KH[0] - 1);
               for (i = 2; i >= BS_KHTindex; i--) {
                  BS_count = BS_count + KeysHistoryTranslation[0][i];
               }
            }
#ifdef DEBUG
            sprintf(str_debug, "KHT[0][3]: %d KHT[0][2]: %d KHT[0][1]: %d KHT[0][0]: %d BS_count: %d\n", KeysHistoryTranslation[0][3], KeysHistoryTranslation[0][2], KeysHistoryTranslation[0][1], KeysHistoryTranslation[0][0], BS_count);
            log_string(str_debug);
            sprintf(str_debug, "KHT[1][3]: %d KHT[1][2]: %d KHT[1][1]: %d KHT[1][0]: %d\n", KeysHistoryTranslation[1][3], KeysHistoryTranslation[1][2], KeysHistoryTranslation[1][1], KeysHistoryTranslation[1][0]);
            log_string(str_debug);
#endif

            // Now that we have used the KeysHistoryTranslation. we need to 0
            // them out as those cant be used for the (if) following hits of
            // greater lengths.
            BS_KHTindex = 3 - (KH[0] - 1);
            for (i = 2; i >= BS_KHTindex; i--) {
               KeysHistoryTranslation[0][i] = 0;
               KeysHistoryTranslation[1][i] = 0;
            }
         

            // Here is where we delete characters already translated before.
            memset(Input, 0, sizeof(Input));
            for (i = 0; i < BS_count; i++) {
               Input[i * 2].type = INPUT_KEYBOARD;
               Input[i * 2].ki.wVk = VK_BACKSPACE;
               Input[i * 2].ki.wScan = SCAN_BACKSPACE;

               Input[i * 2 + 1].type = INPUT_KEYBOARD;
               Input[i * 2 + 1].ki.wVk = VK_BACKSPACE;
               Input[i * 2 + 1].ki.wScan = SCAN_BACKSPACE;
               Input[i * 2 + 1].ki.dwFlags = KEYEVENTF_KEYUP;
            }
            SendInputRetVal = SendInput(2 * BS_count, Input, sizeof(INPUT));
#ifdef DEBUG
            sprintf(str_debug, "Sending %d Backspace press and releases as UNICODE. SendInput returned: %d\n", BS_count * 2, SendInputRetVal);
            log_string(str_debug);
#endif
         }

	// Test the theory that we need to release the Alt Key before injection.
	// Yes, it indeed is true.
	if (AltOn) {
           memset(Input, 0, sizeof(Input));
           Input[0].type = INPUT_KEYBOARD;
           Input[0].ki.wScan = SCAN_ALT;
           if (AltOnR)
              Input[0].ki.dwFlags = KEYEVENTF_SCANCODE|KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP;
           else
              Input[0].ki.dwFlags = KEYEVENTF_SCANCODE|KEYEVENTF_KEYUP;
           SendInputRetVal = SendInput(1, Input, sizeof(INPUT));
#ifdef DEBUG
           sprintf(str_debug, "Injecting (Alt release) extended: %d\n", AltOnR);
           log_string(str_debug);
#endif
	}

         // Lets inject using the SendInput() function.
         // We now inject the RHS for this LHS.
         memset(Input, 0, sizeof(Input));
         for (i = 0; i < count; i++) {
            Input[2 * i].type = INPUT_KEYBOARD;
            Input[2 * i].ki.wScan = RHS[i];
            Input[2 * i].ki.dwFlags = KEYEVENTF_UNICODE;

            Input[2 * i + 1].type = INPUT_KEYBOARD;
            Input[2 * i + 1].ki.wScan = RHS[i];
            Input[2 * i + 1].ki.dwFlags = KEYEVENTF_UNICODE|KEYEVENTF_KEYUP;
         }
         SendInputRetVal = SendInput(2 * count, Input, sizeof(INPUT));
#ifdef DEBUG
         sprintf(str_debug, "Injecting %d (press and release) unicode. SendInput UNICODE returned %d\n", 2 * count, SendInputRetVal);
         log_string(str_debug);
#endif

	// If we had released the Alt key as part of the injection
	// restore it. ie, Press it back again
	if (AltOn) {
           memset(Input, 0, sizeof(Input));
           Input[0].type = INPUT_KEYBOARD;
           Input[0].ki.wScan = SCAN_ALT;
           if (AltOnR)
              Input[0].ki.dwFlags = KEYEVENTF_SCANCODE|KEYEVENTF_EXTENDEDKEY;
           else
              Input[0].ki.dwFlags = KEYEVENTF_SCANCODE;
           SendInputRetVal = SendInput(1, Input, sizeof(INPUT));
#ifdef DEBUG
           sprintf(str_debug, "Injecting (Alt press) extended: %d\n", AltOnR);
           log_string(str_debug);
#endif
	}

         // We eat this keystroke as we have translated it.
         return(-1);

      }
      else {

#ifdef DEBUG
         sprintf(str_debug, "KeyRelease: Translation not spotted\n");
         log_string(str_debug);
#endif

         // just give control to the next in line as we arent
         // translating anything.
         return CallNextHookEx(hhk, code, wParam, lParam);
      }
   }

   // Handle key releases.
   if (keyrelease == 1) {
      // Wonder what we do here.
      return CallNextHookEx(hhk, code, wParam, lParam);
   }
}

 
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fwReason, LPVOID lpvReserved)
{
	hinst = hinstDLL;
	return TRUE;
}

int __cdecl installhook(void)
{
int ErrCode;

   // Update Initial state of the statics.
   // Update static CapsOn with Caps key state.
   if (GetKeyState(VK_CAPITAL) > 0) {
      CapsOn = 1;
   }
   else CapsOn = 0;

   if (GetKeyState(VK_SHIFT) & 0x8000) {
      ShiftOn = 1;
   }
   else ShiftOn = 0;

   if (GetKeyState(VK_CONTROL) & 0x8000) {
      CtrlOn = 1;
   }
   else CtrlOn = 0;

   if (GetKeyState(VK_MENU) & 0x8000) {
      AltOn = 1;
   }
   else AltOn = 0;

#ifdef DEBUG
   sprintf(str_debug, "CapsOn: %x ShiftOn: %x CtrlOn: %x AltOn: %x\n", CapsOn, ShiftOn, CtrlOn, AltOn);
   log_string(str_debug);
#endif

#ifdef DEBUG
   // Write out the Map.
//   DebugWriteMap();
#endif

   if (hhk) return(1);
   hhk = SetWindowsHookEx(WH_KEYBOARD, KHookProc, hinst, 0);
   if (hhk != NULL) {
#ifdef DEBUG
      sprintf(str_debug, "Init: hinst: %p\n", hinst);
      log_string(str_debug);
#endif
   }

   if (hhk == NULL) return(0);
   else {
      HookTranslationEnabled = 1;
      return(1);
   }
}

void __cdecl releasehook(void)
{

   if (!hhk) return;
   UnhookWindowsHookEx(hhk);

#ifdef DEBUG
   sprintf(str_debug, "Exiting\n");
   log_string(str_debug);
#endif

   hhk = NULL;
   HookTranslationEnabled = 0;
}


// Translates the LHS VK codes to the Alt sequence, if possible
// Returns the count of the key codes. 0 => no translation.
// Fills RHS if successful translation.
int TranslateLHS(unsigned int LHS[], unsigned int RHS[7]) {
int index, i;
int count = 0;

#ifdef DEBUG
   sprintf(str_debug, "TranslateLHS: LHS[0]: %d ", LHS[0]);
   log_string(str_debug);
   for (i = 0; i < LHS[0]; i++) {
      sprintf(str_debug, "LHS[%d]: %s LHS[%d]: %s ", 2 * i + 1, BucketCodeToString(LHS[2*i + 1]), 2*i + 2, VKCodeToString(LHS[2*i + 2]));
      log_string(str_debug);
   }
   sprintf(str_debug, "\n");
   log_string(str_debug);
#endif

   // We now see if there is a translation for LHS[];
   index = 0;
   while (1) {

#if 0
      sprintf(str_debug, "TranslateLHS: KeyTrans[%d][0]: %d KeyTrans[%d][1]: %s KeyTrans[%d][2]: %s\n", index, KeyTrans[index][0], index, BucketCodeToString(KeyTrans[index][1]), index, VKCodeToString(KeyTrans[index][2]));
      log_string(str_debug);
#endif

      if (KeyTrans[index][0] == 0) break;

      if (memcmp(LHS, KeyTrans[index], sizeof(unsigned int) * (LHS[0] * 2 + 1)) == 0) {

         // Got a hit. Prepare RHS[]
         count = KeyTrans[index][9]; // Count of key strokes to inject.
         memcpy(RHS, &KeyTrans[index][10], sizeof(unsigned int) * count);

#ifdef DEBUG
         sprintf(str_debug, "Rule Hit -> count: %d index: %d\n", count, index);
         log_string(str_debug);
         for (i = 0; i < count; i++) {
            sprintf(str_debug, "RHS[%d]: %d ", i, RHS[i]);
            log_string(str_debug);
         }
         sprintf(str_debug, "\n");
         log_string(str_debug);
#endif
         break;
      }
      index++;
   }
   return(count);
}


// Interface used by Main program to get the state of the
// HookTranslationEnabled variable.
int GetHookTranslationEnabled() {

#ifdef DEBUG
   sprintf(str_debug, "Dll: GetHookTranslationEnabled(): %d\n", HookTranslationEnabled);
   log_string(str_debug);
#endif
   return(HookTranslationEnabled);
}

// Interface used by Main program to set the state of the
// HookTranslationEnabled variable.
void SetHookTranslationEnabled(int set_val) {

#ifdef DEBUG
   sprintf(str_debug, "Dll: SetHookTranslationEnabled(): %d\n", set_val);
   log_string(str_debug);
#endif
   HookTranslationEnabled = set_val;
}
