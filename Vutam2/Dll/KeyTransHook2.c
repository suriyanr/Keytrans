// This is the mode toggle implementation using the CTRL key.


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// This gives us the global KeyTrans[][8].
// Also the global VKScan[][2]. Translate VK to Scan.
// Both the arrays end with 0s in last row.
#include "KeyTransHook2.h" 


#include <stdio.h>

#if 0 // Turn it to 1 to turn debug on.
#define DEBUG 1
static char str_debug[512];
#endif

#define VK_CTRL          0x11
#define VK_ALT           0x12
#define VK_CAPSLOCK      0x14
#define VK_BACKSPACE     0x08
#define SCAN_CTRL        0x1D
#define SCAN_ALT         0x38
#define SCAN_BACKSPACE   0x0E

// Mappings will be under "[Mappings]"
// Ligatures will be under "[Ligatures]"
#define INIFILE      ".\\KeyTrans2.ini"
#define MAPPINGS     "Mappings"
#define LIGATURES    "Ligatures"

static HHOOK hhk = NULL;
static HINSTANCE hinst; 

// The global key state statics.
static int CapsOn = 0;
static int ShiftOn = 0;
static int CtrlOn = 0;
static int AltOn = 0;

// History of 3 last characters.
// Each character takes two bytes.
// First byte = BUCKET (BUCKET1|BUCKET2|BUCKET3)
// Second byte = VK_Code
// This is exactly the same how LigatureMap[even int][0 to 6] is stored.
// The most recent is in index 4 and 5.
// Oldest is in index 0, 1.
// 7 is the NULL termination.
static unsigned char KeysHistory[7] = { 0, 0, 0, 0, 0, 0, 0 };

#ifdef DEBUG
void log_string() {
FILE *log_file;

   // Lets log it.
   log_file = fopen("c:\\logger.txt", "a+");
   fprintf(log_file, "%s", str_debug);
   fclose(log_file);
}
#endif

// Writes out the default mapping into file INIFILE.
void write_default_map() {
FILE *map_file;
int i, rule;
int cnt, j;

   map_file = fopen(INIFILE, "w");
   if (map_file == NULL) return;

   fprintf(map_file, "["MAPPINGS"]\n");

   i = 0;
   while (1) {
      if (IniStrArray[i][0] == '\0') break;

      fprintf(map_file, IniStrArray[i]);
      fprintf(map_file, "=");
      cnt = KeyTrans[i][1]; // get the count.
      for (j = 0; j < cnt; j++) {
         fprintf(map_file, "%c", KeyTrans[i][j + 2] + '0');
      }
      fprintf(map_file, "\n");
      i++;
   }
   fprintf(map_file, "\n");

   fprintf(map_file, "["LIGATURES"]\n");
   i = 0;
   rule = 1;
   while (1) {
   char str_rule[7];

      if (LigatureMap[i][0] == '\0') break;
      sprintf(str_rule, "Rule%d", rule++);
      fprintf(map_file, str_rule);
      fprintf(map_file, "=");
 
      // Write out the Bucket and VK info
      j = 0;
      while (1) {
         if (LigatureMap[i][j] == '\0') break;
         if (j == 8) break;
         fprintf(map_file, BucketCodeToString(LigatureMap[i][j]));
         fprintf(map_file, " ");
         j++;

         fprintf(map_file, VKCodeToString(LigatureMap[i][j]));
         fprintf(map_file, " ");
         j++;
      }

      i++;
      // Write out the alt string.
      cnt = LigatureMap[i][0]; // get the count.
      for (j = 0; j < cnt; j++) {
         fprintf(map_file, "%c", LigatureMap[i][j + 1] + '0');
      }
      fprintf(map_file, "\n");

      i++;
   }

   fprintf(map_file, "\n");
   fclose(map_file);
}

// Function which return the VK String when passed the VK code.
// See the header comment on VKStringToCode() as to how its done.
const char *VKCodeToString(unsigned char VKCode) {
const char *VKString = NULL;
int index;

   index = 0;
   while (1) {
      if (KeyTrans[index][0] == 0) break;

      if (KeyTrans[index][0] == VKCode) {
         VKString = IniStrArray[index];
         break;
      }
      index++;
   }
   return(VKString);
}


// Function which returns the VK code when passed the VK String.
// We make use of the two arrays: *IniStrArray[], and KeyTrans[][8].
// *IniStrArray[] consists of the string names of the VK Keys.
// KeyTrans[][0] contains the VK values of the keys in the same order
// as present in *IniStrArray[]
unsigned char VKStringToCode(char *VKString) {
unsigned char vk_key = 0;
int index;

   index = 0;
   while (1) {
      if (IniStrArray[index][0] == '\0') break;

      if (strcasecmp(VKString, IniStrArray[index]) == 0) {
         vk_key = KeyTrans[index][0];
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
   return(bucket_code);
}

// If INIFILE exists, try to get the map.
// We overwrite the values already present in KeyTrans[][8] as we read
// from the INIFILE.
// The strings to read are in array: IniStrArray[], ends with ""
void initialise_map() {
char str[1024];
int index;
int i;
int keytrans_index;

   index = 0;
   while (1) {
#ifdef DEBUG
      sprintf(str_debug, "Init_Map: Orig-> String: %s KeyTrans: %x %x %x %x %x %x %x %x\n", IniStrArray[index], KeyTrans[index][0], KeyTrans[index][1], KeyTrans[index][2], KeyTrans[index][3], KeyTrans[index][4], KeyTrans[index][5], KeyTrans[index][6], KeyTrans[index][7]);
      log_string();
#endif
      if (IniStrArray[index][0] == '\0') break;

      if (GetPrivateProfileString(MAPPINGS, IniStrArray[index], "", str, sizeof(str), INIFILE) > 0) {
         // Update KeyTrans[index][...] with str.
         i = 0;
         keytrans_index = 2; // 0 is the VK, 1 is count, 2 onwards is vk trans.
                             // keytrans_index can go upto 7, inclusive.
         while (str[i] != '\0') {
            if ( (str[i] >= '0') && (str[i] <= '9') ) {
               KeyTrans[index][keytrans_index] = str[i] - '0';
               keytrans_index++;
               if (keytrans_index == 8) break;
            }
            i++;
         }
         // Update the count.
         KeyTrans[index][1] = keytrans_index - 2;
#ifdef DEBUG
         sprintf(str_debug, "Init_Map: Read: %s KeyTrans: %x %x %x %x %x %x %x %x\n", str, KeyTrans[index][0], KeyTrans[index][1], KeyTrans[index][2], KeyTrans[index][3], KeyTrans[index][4], KeyTrans[index][5], KeyTrans[index][6], KeyTrans[index][7]);
         log_string();
#endif
      }
      index++;
   }

   // Lets read in the Ligature Rules. (max LIGATURES_MAX)
   index = 0;
   while (index < 2 * LIGATURES_MAX) {
   char str_rules[7];
   char *str_copy;
   char *str_space;
   char *str_process;

      sprintf(str_rules, "Rule%d", index);
      if (GetPrivateProfileString(LIGATURES, str_rules, "", str, sizeof(str), INIFILE) > 0) {
         // Update LigatureMap[index, index + 1][...] with str.
         str_copy = strdup(str);
         str_process = str_copy;

         // zero out the array.
         memset(LigatureMap[index], 0, sizeof(char) * 7);
         memset(LigatureMap[index + 1], 0, sizeof(char) * 7);

         // We scan the line word by word.
         i = 0;
         while (1) {
            str_space = strstr(str_process, " ");
            if (str_space == NULL) break;

            *str_space = '\0'; // str_process now contains BUCKET word.
                               // or last word = Alt key string

            // First lets check if this is the last word = Alt Key String.
            if (strspn(str_process, "0123456789") == strlen(str_process)) {
               // This is the last word = Alt Key String.
               index++;
               keytrans_index = 1;
               while (*str_process != '\0') {
                  LigatureMap[index][keytrans_index] = (*str_process) - '0';
                  keytrans_index++;
                  if (keytrans_index == 7) break;
                  str_process++;
               }
               LigatureMap[index][0] = keytrans_index - 1;
               index++;
               break; // Done with scanning a Rule.
            }

            LigatureMap[index][i] = BucketStringToCode(str_process);
            i++;

            str_process = str_space;
            str_process++; // Move past the \0

            str_space = strstr(str_process, " ");
            if (str_space == NULL) break; // syntax error in ini file.

            *str_space = '\0'; // str_process now contains VK_STRING
            LigatureMap[index][i] = VKStringToCode(str_process);
            i++;

            str_process = str_space;
            str_process++; // Move past the \0
         }
         free(str_copy);
      }
      else break; // No more rules.
   }
}

unsigned char scan_code(unsigned char vk) {
int index;
unsigned char scan = 0;

   index = 0;
   do {
      if ( (VKScan[index][0] == 0) && (VKScan[index][1] == 0) ) break;
      if (VKScan[index][0] == vk) {
         scan = VKScan[index][1];
         break;
      }
      index++;
   } while(1);
   return(scan);
}

// If Ctrl and alt is pressed and a key is pressed, we translate.
static LRESULT CALLBACK KHookProc(int code, WPARAM wParam, LPARAM lParam)
{
int keyrelease;
unsigned char vk;

   // if code != 0 just do normal stuff.
   if (code != 0) {
      return CallNextHookEx(hhk, code, wParam, lParam);
   }

//   repeatcount = lParam & 0x0ffff;
//   scancode    = (lParam & 0xff0000) >> 16;
//   extended    = (lParam & 0x1000000) >> 24;
//   alt         = (lParam & 0x20000000) >> 29;
//   prestate    = (lParam & 0x40000000) >> 30;
   keyrelease  = (lParam & 0x80000000) >> 31;

   vk = wParam;

   // Remember state of Caps lock key.
   if (vk == VK_CAPITAL) {
      if (keyrelease == 1) {
         if (CapsOn == 1) CapsOn = 0;
         else CapsOn = 1;
      }
   }

   // Remember state of Ctrl Key.
   if ( (vk == VK_CONTROL) || (vk == VK_LCONTROL) || (vk == VK_RCONTROL) ) {
      if (keyrelease == 1) {
         CtrlOn = 0;
      }
      else CtrlOn = 1;
   }

   // Remember state of Shift Key.
   if ( (vk == VK_SHIFT) || (vk == VK_LSHIFT) || (vk == VK_RSHIFT) ) {
      if (keyrelease == 1) {
         ShiftOn = 0;
      }
      else ShiftOn = 1;
   }

   // Remember state of Alt Key.
   if ( (vk == VK_MENU) || (vk == VK_LMENU) || (vk == VK_RMENU) ) {
      if (keyrelease == 1) {
         AltOn = 0;
      }
      else AltOn = 1;
   }


   // We process only if the key is present in KeyTrans[][0]
   // All other releases or presses we ignore.
   if (CheckVKCode(vk) == 0) {
      return CallNextHookEx(hhk, code, wParam, lParam);
   }

   // Handle key releases.
   if (keyrelease == 1) {
   int i, count;
   unsigned char key[6], s_key[6];

      // Now we check if it matches a ligature.
      // We eat the key if it matches the ligature, as the key release code
      // will inject the ligature.

      // First check if the longest ligature matches. (3 keystrokes)
      count = TranslateLigature(KeysHistory, key, s_key);
      if (count == 0) {
         // Now check if the shorter ligature works. (2 keystrokes)
         count = TranslateLigature(&KeysHistory[2], key, s_key);
      }
      if (count != 0) {
#ifdef DEBUG
         sprintf(str_debug, "KeyRelease: Ligature spotted: count: %x\n", count);
         log_string();
#endif

         // Note we can come here with Ctrl and alt pressed.
         // That is a bucket 3 key has been pressed, which forms a ligature.
         if (CtrlOn) {
            // Release Ctrl.
            keybd_event(VK_CTRL, SCAN_CTRL, KEYEVENTF_KEYUP, 0);
         }

         if (AltOn) {
            // Release Alt.
            keybd_event(VK_ALT, SCAN_ALT, KEYEVENTF_KEYUP, 0);
         }

         // We inject this ligature.
         // Press BackSpace
         keybd_event(VK_BACKSPACE, SCAN_BACKSPACE, 0, 0);

         // Release BackSpace
         keybd_event(VK_BACKSPACE, SCAN_BACKSPACE, KEYEVENTF_KEYUP, 0);

         // Press Alt.
         keybd_event(VK_ALT, SCAN_ALT, 0, 0);

         for (i = 0; i < count; i++) {
            keybd_event(key[i], s_key[i], 0, 0);
            keybd_event(key[i], s_key[i], KEYEVENTF_KEYUP, 0);
         }

         // Release Alt.
         keybd_event(VK_ALT, SCAN_ALT, KEYEVENTF_KEYUP, 0);

         // Restore original Ctrl and Alt state of key.
         if (CtrlOn) {
            // Press Ctrl.
            keybd_event(VK_CTRL, SCAN_CTRL, 0, 0);
         }

         if (AltOn) {
            // Press Alt.
            keybd_event(VK_ALT, SCAN_ALT, 0, 0);
         }

         // We eat this keyrelease as its a ligature.
         return(-1);
      }

      // Translate the key if Ctrl and Alt are pressed.
      if (CtrlOn && AltOn) {
      int count, i;
      unsigned char key[6], s_key[6];

         // Release Ctrl.
         keybd_event(VK_CTRL, SCAN_CTRL, KEYEVENTF_KEYUP, 0);

         // Release Alt.
         keybd_event(VK_ALT, SCAN_ALT, KEYEVENTF_KEYUP, 0);

         // We now see if there is a translation present
         count = TranslateVKCode(vk, key, s_key);

         // Press Alt.
         keybd_event(VK_ALT, SCAN_ALT, 0, 0);

         for (i = 0; i < count; i++) {
            keybd_event(key[i], s_key[i], 0, 0);
            keybd_event(key[i], s_key[i], KEYEVENTF_KEYUP, 0);
         }

         // Release Alt.
         keybd_event(VK_ALT, SCAN_ALT, KEYEVENTF_KEYUP, 0);

         // Press Ctrl.
         keybd_event(VK_CTRL, SCAN_CTRL, 0, 0);

         // Press Alt.
         keybd_event(VK_ALT, SCAN_ALT, 0, 0);

         return(-1);
      }
   }

   // Handle key pressses.
   if (keyrelease == 0) {
   int i;
   unsigned char bucket, key[6], s_key[6];
   int count;

      // Lets log the key.
      // Move index 2, 3, 4, 5 onto index 0, 1, 2, 3
      for (i = 0; i < 4; i++) {
         KeysHistory[i] = KeysHistory[i + 2];
      }
      // Lets get the bucket.
      // If Ctrl pressed and Alt pressed = bucket3.
      // If Capslock is off and shift not pressed = bucket1
      // If Capslock is on and shift not pressed = bucket2
      // If Capslock is on and shift is pressed = bucket1
      // If Capslock is off and shift is pressed = bucket2

      // Initialise bucket to BUCKET1
      bucket = BUCKET1;

      if ( (CtrlOn == 1) && (AltOn == 1) ) {
         bucket = BUCKET3;
      }
      else if ( (CapsOn == 0) && (ShiftOn == 0) ) {
         bucket = BUCKET1;
      }
      else if ( (CapsOn == 1) && (ShiftOn == 0) ) {
         bucket = BUCKET2;
      }
      else if ( (CapsOn == 1) && (ShiftOn == 1) ) {
         bucket = BUCKET1;
      }
      else if ( (CapsOn == 0) && (ShiftOn == 1) ) {
         bucket = BUCKET2;
      }
      KeysHistory[4] = bucket;
      KeysHistory[5] = vk;

#ifdef DEBUG
      sprintf(str_debug, "KeysHistory: %x %x %x %x %x %x\n", KeysHistory[0], KeysHistory[1], KeysHistory[2], KeysHistory[3], KeysHistory[4], KeysHistory[5]);
      log_string();
#endif

      // Now we check if it matches a ligature.
      // We eat the key if it matches the ligature, as the key release code
      // will inject the ligature.
      count = TranslateLigature(KeysHistory, key, s_key);
      if (count == 0) {
         count = TranslateLigature(&KeysHistory[2], key, s_key);
      }
      if (count != 0) {
         // We eat this keypress as its a ligature.
#ifdef DEBUG
         sprintf(str_debug, "KeyPress: Ligature spotted: count: %x\n", count);
         log_string();
#endif
         return(-1);
      }

      // We eat a key press if Ctrl and Alt are pressed.
      if (CtrlOn && AltOn) {
#ifdef DEBUG
         sprintf(str_debug, "Eating key press. vk: %x\n", vk);
         log_string();
#endif
         return(-1);
      }
   }

   return CallNextHookEx(hhk, code, wParam, lParam);
}

 
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fwReason, LPVOID lpvReserved)
{
	hinst = hinstDLL;
	return TRUE;
}

int __cdecl installhook(void)
{
#ifdef DEBUG
FILE *log_file;

   log_file = fopen("c:\\logger.txt", "w");
   fclose(log_file);
#endif

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
   log_string();
#endif

   initialise_map(); // Populate KeyTrans[][8] if ini file present.
   if (hhk) return(1);
   hhk = SetWindowsHookEx(WH_KEYBOARD, KHookProc, hinst, 0);
   if (hhk != NULL) {
#ifdef DEBUG
      sprintf(str_debug, "Init: hinst: %p\n", hinst);
      log_string();
#endif
   }

   if (hhk == NULL) return(0);
   else return(1);
}

void __cdecl releasehook(void)
{

   if (!hhk) return;
   UnhookWindowsHookEx(hhk);

#ifdef DEBUG
   sprintf(str_debug, "Exiting\n");
   log_string();
#endif

   hhk = NULL;
}


// This function returns 1 if its a VK code we process.
// else returns 0.
// Basically exists in KeyTrans[][0]
int CheckVKCode(unsigned char vk) {
int index;
int present = 0;

   index = 0;
   while (1) {
      if (KeyTrans[index][0] == 0) break;

      if (KeyTrans[index][0] == vk) {
         present = 1;
         break;
      }
      index++;
   }
   return(present);
}


// Translates the VK code to the Alt sequence.
// Returns the count of the key codes.
// Fills key with the VK codes. (numeric key pad)
// Fills s_key with the Scan codes. (numeric key pad)
int TranslateVKCode(unsigned char vk, unsigned char key[6], unsigned char s_key[6]) {
int index, i;
int count = 0;

   // We now see if there is a translation present in KeyTrans[][8];
   index = 0;
   while (1) {
      if ((KeyTrans[index][0] == 0) && (KeyTrans[index][1] == 0)) break;

      if (vk == KeyTrans[index][0]) {

         // Got a hit. Prepare key[] and s_key[]
         count = KeyTrans[index][1]; // Count of key strokes to inject.
         for (i = 0; i < count; i++) {
            key[i] = KeyTrans[index][i+2];
            key[i] += VK_NUMPAD0;
            s_key[i] = scan_code(key[i]);
         }

#ifdef DEBUG
         {
         char temp_buf[256];
         sprintf(str_debug, "Translation -> vk: %x index: %x KeyTrans[index][0]: %x count: %x\n", vk, index, KeyTrans[index][0], KeyTrans[index][1]);
         log_string();
         sprintf(str_debug, "Translation -> ");
         for (i = 0; i < count; i++) {
            sprintf(temp_buf, "key[%d]: %x ", i, key[i]);
            strcat(str_debug, temp_buf);
         }
         for (i = 0; i < count; i++) {
            sprintf(temp_buf, "s_key[%d]: %x ", i, s_key[i]);
            strcat(str_debug, temp_buf);
         }
         strcat(str_debug, "\n");
         log_string();
         }
#endif
         break;
      }
      index++;
   }
   return(count);
}

// Translates the Ligature VK codes to the Alt sequence, if possible.
// LMap is null terminated.
// Returns the count of the key codes. 0 => no translation.
// Fills key with the VK codes. (numeric key pad)
// Fills s_key with the Scan codes. (numeric key pad)
int TranslateLigature(unsigned char LMap[], unsigned char key[6], unsigned char s_key[6]) {
int index, i;
int count = 0;

   // We now see if there is a translation present in LigatureMap[501][7];
   index = 0;
   while (1) {
      if ((LigatureMap[index][0] == 0) && (LigatureMap[index][1] == 0)) break;

      if (strcmp(LMap, LigatureMap[index]) == 0) {

         // Got a hit. Prepare key[] and s_key[]
         index++;
         count = LigatureMap[index][0]; // Count of key strokes to inject.
         for (i = 0; i < count; i++) {
            key[i] = LigatureMap[index][i+1];
            key[i] += VK_NUMPAD0;
            s_key[i] = scan_code(key[i]);
         }
#ifdef DEBUG
         sprintf(str_debug, "Ligature Hit -> count: %x index: %x\n", count, index - 1);
         log_string();
#endif
         break;
      }
      index = index + 2;
   }
   return(count);
}

