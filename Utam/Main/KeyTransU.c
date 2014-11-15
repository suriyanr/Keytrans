#include <windows.h>

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

//#include <shlobj.h>

// Borrow below from KeyTrans2HookU.c
#define MAXMAPENTRIES 1000

// Set it to 0, to turn off DEBUG.
#if 0
#define DEBUG 1
#endif

#ifdef DEBUG
char str_debug[512];
#endif

#define VERSION "October 20, 2008 - Version 3.0\n"

#define TRAY_MSG        WM_APP
#define SWM_ENABLE      WM_APP + 1   // Enable the hook
#define SWM_DISABLE     WM_APP + 2   // Disable the hook
#define SWM_EXIT        WM_APP + 3   // Exit program

// Message numbers for the (max) 10 map files.
#define SWM_MAPFILE_START    WM_APP + 4

#ifndef NOTIFYICONDATA_V1_SIZE
# define NOTIFYICONDATA_V1_SIZE 88
#endif

#ifndef NOTIFYICONDATA_V2_SIZE
# define NOTIFYICONDATA_V2_SIZE 488
#endif

#ifndef NOTIFYICONDATA_V3_SIZE
# define NOTIFYICONDATA_V3_SIZE 504
#endif

#define IDI_MYICON 201 // Got from Icon.rc file.
#define TRAYICONID 1

#define MAP_FILE    "KeyTransU.map"

NOTIFYICONDATA  niData;
int HookEnabled;

// This holds the current successfully loaded map File name.
char *MapFileName = NULL;

// Max number of map files we show in the menu.
#define MAX_MAP_FILES 10

// Number of Map Files shown on the popup.
int MapFileCount = 0;

// The names of the map files we are displaying in the menu.
char *MapFileNames[MAX_MAP_FILES] = {
NULL, NULL, NULL, NULL, NULL,
NULL, NULL, NULL, NULL, NULL };

void ShowContextMenu(HWND hWnd);
//int GetFileNameFromUser();
int LoadMapFile(char *filename);

extern int __cdecl installhook(void);
extern void __cdecl releasehook(void);


/* global semaphore so only one instance can run. */
HANDLE OnlyOneSem = NULL;

/* Returns 1 if already running, else returns 0. */
int CheckIfRunning();

LRESULT CALLBACK WProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
int wmId, wmEvent;
int MapFileIndex;

   switch (uMsg) {
      case WM_DESTROY:
         niData.uFlags = 0;
         Shell_NotifyIcon(NIM_DELETE,&niData);
         if (HookEnabled == 1) {
            HookEnabled = 0;
            releasehook();
         }
         PostQuitMessage(0);
         return(0);
         break;

      case TRAY_MSG:
        switch (lParam) {
           case WM_RBUTTONDOWN:
           case WM_CONTEXTMENU:
              ShowContextMenu(hWnd);
              break;
        }
        break;

      case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);

        switch (wmId) {
           case SWM_ENABLE:
#if 0
              if (HookEnabled == 0) {
                 if (installhook()) {
                    HookEnabled = 1;
                 }
              }
#endif
#ifdef DEBUG
              sprintf(str_debug, "SWM_ENABLE: SetHookTranslationEnabled(1)\n");
              log_string(str_debug);
#endif
              SetHookTranslationEnabled(1);
              break;

           case SWM_DISABLE:
#if 0
              if (HookEnabled == 1) {
                 releasehook();
                 HookEnabled = 0;
              }
#endif
#ifdef DEBUG
              sprintf(str_debug, "SWM_DISABLE: SetHookTranslationEnabled(0)\n");
              log_string(str_debug);
#endif
              SetHookTranslationEnabled(0);
              break;

           case SWM_EXIT:
              DestroyWindow(hWnd);
              break;

           default: 
              // Here we check if its  greater than equal to SWM_MAPFILE_START
              // and less than SWM_MAPFILE_START + MapFileCount
              MapFileIndex = wmId - SWM_MAPFILE_START - 8;
              // OK, I have no idea but MapFileIndex gives values 8 more than
              // than it should be.
#ifdef DEBUG
              sprintf(str_debug, "default: wmId: %d SWM_MAPFILE_START: %d MapFileCount: %d MapFileIndex: %d wmId - SWM_MAPFILE_START: %d\n", wmId, SWM_MAPFILE_START, MapFileCount, MapFileIndex, wmId - SWM_MAPFILE_START);
              log_string(str_debug);
#endif
              if ( (MapFileIndex >= 0) && (MapFileIndex < MapFileCount) ) {
                 // We should load the appropriate map file.
                 if (MapFileName) free(MapFileName);
                 MapFileName = malloc(strlen(MapFileNames[MapFileIndex]) + 1);
                 strcpy(MapFileName, MapFileNames[MapFileIndex]);

                 // Load the Map from the file specified in MapFileName
                 if (LoadMapFile(MapFileName) != 1) {
                    // We did not load it from MapFileName, free it!
                    free(MapFileName);
                    MapFileName = NULL;
                 }
              }
              break;
        }
        break;
   }

   return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
WNDCLASSEX wcx;
MSG msg;
HWND hWnd;
BOOL ret;
char my_msg[1024];
int ErrCode;

#ifdef DEBUG
char DebugFileName[512];

   // Lets populate DebugFile, so its useful.
   getcwd(DebugFileName, 500);
   strcat(DebugFileName, "\\logger.txt");
   SetDebugFile(DebugFileName);
#endif

   if ( (strcasecmp(lpCmdLine, "/?") == 0) || 
             (strcasecmp(lpCmdLine, "/h") == 0) ) {
      // We give the help syntax.
      sprintf(my_msg, 
                "KeyTransU © 2008 - Suriyan Ramasami.\n"
                "UTAM Font © 2008 - V. Ramasami.\n"
                VERSION
                "\n"
                "KeyTransU /h\t:Help\n\n"
                "Initialization file syntax:\n"
                "\tLHS = RHS\n"
                "\twhere LHS = Virtual Key Strokes\n"
                "\twhere Virtual Key Stroke = BUCKET<1|2|3|4> VK_KEY*\n"
                "\twhere RHS=unicode character codes\n"
                "\tExample Rule:\n"
                "\t\tBUCKET1 VK_BRA_OPEN BUCKET2 VK_J = 2965 3021 2999 3014\n"
                "\tAnother simpler example:\n"
                "\t\tBUCKET1 VK_A = 2949\n"
             );
      MessageBox(NULL, my_msg, "KeyTransU", MB_OK);
      return(2);
   }

   /* Only one invocation allowed. */
   /* check if we are already running. */
   if (CheckIfRunning()) {
      MessageBox(NULL, "Only one instance allowed.", "Error", MB_OK);
      return(-1);
   }

   wcx.cbSize = sizeof(WNDCLASSEX);
   wcx.style  = 0;
   wcx.lpfnWndProc = WProc;
   wcx.cbClsExtra = 0;
   wcx.cbWndExtra = 0;
   wcx.hInstance = hInst;
   wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
   wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
   wcx.hbrBackground = (HBRUSH)COLOR_BTNFACE;
   wcx.lpszMenuName = NULL;
   wcx.lpszClassName = "KeyTransU";
   wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
   if (!RegisterClassEx(&wcx)) {
      MessageBox(NULL, "Failed registering window class","KeyTransU",MB_OK);
      return(-1);
   }

   hWnd = CreateWindow(
                "KeyTransU",
                "KeyTransU",
                WS_OVERLAPPED | WS_SYSMENU,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                NULL,
                NULL,
                hInst,
                0);
   if (!hWnd) {
      MessageBox(NULL, "Failed opening window","KeyTransU",MB_OK);
      return(-1);
   }

   // Tray Bar Icon.
   ZeroMemory(&niData,sizeof(NOTIFYICONDATA));
   niData.cbSize = sizeof(NOTIFYICONDATA);
//   niData.cbSize = NOTIFYICONDATA_V1_SIZE;
   niData.uID = TRAYICONID;
   niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
   // niData.hIcon = LoadIcon (NULL, IDI_APPLICATION);
   niData.hIcon = LoadIcon (hInst, MAKEINTRESOURCE(IDI_MYICON));

   niData.hWnd = hWnd;
   niData.uCallbackMessage = TRAY_MSG;
   strcpy(niData.szTip, "KeyTransU - UTAM");
   Shell_NotifyIcon(NIM_ADD, &niData);

   if(niData.hIcon && DestroyIcon(niData.hIcon)) {
      niData.hIcon = NULL;
   }

   
   MapFileName = malloc(strlen(MAP_FILE) + 1);
   strcpy(MapFileName, MAP_FILE);

   // Load the Map from the file specified in MapFileName
   if (LoadMapFile(MapFileName) != 1) {
      // We did not load it from MapFileName, free it!
      free(MapFileName);
      MapFileName = NULL;
   }

   if (installhook()) {
      HookEnabled = 1;
   }
   else HookEnabled = 0;

   while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0) {
      if (ret == -1) {

      } else {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
   }

   if (HookEnabled == 1) {
      // uninstall hook
      HookEnabled = 0;
      releasehook();
   }

   /* Destroy the Semaphore. */
   CloseHandle(OnlyOneSem);

   return(msg.wParam);
}

/* Returns 1 if already running, else returns 0. */
int CheckIfRunning() {

   // Only one of Keytrans/Keytrans2/KeytransU can be running.
   // Hence we keep using the same Vutam Named Semaphore.
   OnlyOneSem = CreateSemaphore(NULL, 0, 1, "Vutam");
   if (OnlyOneSem == NULL) {
      // Semaphore already exists.
      return(1);
   }

   if (GetLastError() == ERROR_ALREADY_EXISTS) {
      // Semaphore already exists.
      return(1);
   }
   return(0);
}

void ShowContextMenu(HWND hWnd) {
POINT pt;
HMENU hMenu;
HMENU hMapMenu;
DIR *d;
struct dirent *dir;

   GetCursorPos(&pt);
   hMenu = CreatePopupMenu();
   hMapMenu = CreatePopupMenu();

   MapFileCount = 0;
   if (hMapMenu) {
      // We read the current directory (KeyTransU invoked directory)
      // and scan for all .map files. We list those files here.
      // List the first MAX_MAP_FILES only.
      d = opendir(".");
      if (d) {
         while ((dir = readdir(d)) != NULL) {
            if (strcasecmp(&dir->d_name[dir->d_namlen - 4], ".map") == 0) {
            UINT Uflags;
               // It ends with .map
               if (MapFileNames[MapFileCount]) {
                  free(MapFileNames[MapFileCount]);
               }
               MapFileNames[MapFileCount] = malloc(dir->d_namlen + 1);
               strcpy(MapFileNames[MapFileCount], dir->d_name);
               if ((MapFileName) &&
                   (strcasecmp(MapFileNames[MapFileCount], MapFileName) == 0)) {
                  // We need to put a check mark as its the loaded one.
                  Uflags = MF_BYPOSITION | MF_CHECKED;
               }
               else {
                  // No check mark for this one.
                  Uflags = MF_BYPOSITION;
               }
               InsertMenu(hMapMenu, -1, Uflags, SWM_MAPFILE_START + MapFileCount, MapFileNames[MapFileCount]);
#ifdef DEBUG
               sprintf(str_debug, "InsertMenu: MapFileNames[%d]: %s SWM_ID: %d\n", MapFileCount, MapFileNames[MapFileCount], SWM_MAPFILE_START + MapFileCount);
               log_string(str_debug);
#endif
               MapFileCount++;
               if (MapFileCount == MAX_MAP_FILES) break;
            }
         }
         closedir(d);
      }
   }

   if (hMenu) {
      if (HookEnabled == 1) {
         int TranslationEnabled;
         char my_string[512];

         TranslationEnabled = GetHookTranslationEnabled();
#ifdef DEBUG
         sprintf(str_debug, "Menu population: TranslationEnabled: %d\n", TranslationEnabled);
         log_string(str_debug);
#endif
         if (TranslationEnabled == 1) {
            sprintf(my_string, "Disable - Ctrl SPACE");
            InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_DISABLE, my_string);
         }
         else {
            sprintf(my_string, "Enable  - Ctrl SPACE");
            InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_ENABLE, my_string);
         }
      }
      else {
         // Should not come here. Will come here if hook was not installed.
         InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_ENABLE, "Enable");
      }
      InsertMenu(hMenu, -1, MF_BYPOSITION|MF_SEPARATOR, 0, NULL);
      if (MapFileName) {
         InsertMenu(hMenu, -1, MF_BYPOSITION | MF_DISABLED | MF_CHECKED, 0, MapFileName);
      }
      else {
         InsertMenu(hMenu, -1, MF_BYPOSITION , 0, "Load Map file");
      }
      InsertMenu(hMenu, -1, MF_BYPOSITION|MF_SEPARATOR, 0, NULL);
      InsertMenu(hMenu, -1, MF_BYPOSITION|MF_POPUP, (UINT) hMapMenu,  "Map Files");
      InsertMenu(hMenu, -1, MF_BYPOSITION|MF_SEPARATOR, 0, NULL);
      InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_EXIT, "Exit");

      SetForegroundWindow(hWnd);
      TrackPopupMenu(hMenu, TPM_BOTTOMALIGN,
                     pt.x, pt.y, 0, hWnd, NULL );
      DestroyMenu(hMenu);
   }
}


// Returns 1 on success
// 0 on failure.
int LoadMapFile(char *FileName) {
int ErrCode;
char my_msg[1024];

   ErrCode = InitialiseMap(FileName);
   if (ErrCode != 0) {
      // The Map file has errors.
      if (ErrCode > 0) {
         // If its positive then the error is in that line number.
         sprintf(my_msg, 
                    "Error in file %s at line: %d\n"
                    "Please correct it and reload the map file.\n",
                    FileName, ErrCode);
         MessageBox(NULL, my_msg, "Error", MB_OK);
      }
      else {
         // If its -ve, 
         // -1 => we couldn't open the file successfully.
         // -2 => Number of map entries exceeds MAXMAPENTRIES
         switch (ErrCode) {
 
            case -1:
               sprintf(my_msg,
                       "Error in opening the map file: %s\n",
                       FileName);
               break;

            case -2:
               sprintf(my_msg,
                       "Map file has more than %d entries\n",
                       MAXMAPENTRIES);
               break;

            default:
               sprintf(my_msg,
                      "Unknown error\n");
               break;
         }
         MessageBox(NULL, my_msg, "Error", MB_OK);
      }

      ErrCode = 0;
   }
   else {
      // We are here the map file was read in perfectly OK.
#ifdef DEBUG
      // Write out the KeyTrans array.
      DebugWriteMap();
#endif
      ErrCode = 1;
   }

   return(ErrCode);
}
