#include <windows.h>

#define VERSION "March 10, 2005 - Version 1.0\n"

#define TRAY_MSG        WM_APP
#define SWM_ENABLE      WM_APP + 1   // Enable the hook
#define SWM_DISABLE     WM_APP + 2   // Disable the hook
#define SWM_EXIT        WM_APP + 3   // Exit program

#define NOTIFYICONDATA_V1_SIZE 88
#define NOTIFYICONDATA_V2_SIZE 488
#define NOTIFYICONDATA_V3_SIZE 504
#define IDI_MYICON 201 // Got from Icon.rc file.
#define TRAYICONID 1

#define INIFILE      ".\\KeyTrans.ini"


NOTIFYICONDATA  niData;
int HookEnabled;

void ShowContextMenu(HWND hWnd);

extern void __cdecl write_default_map(void);
extern int __cdecl installhook(void);
extern void __cdecl releasehook(void);


/* global semaphore so only one instance can run. */
HANDLE OnlyOneSem = NULL;

/* Returns 1 if already running, else returns 0. */
int CheckIfRunning();

LRESULT CALLBACK WProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
int wmId, wmEvent;

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
              if (HookEnabled == 0) {
                 if (installhook()) {
                    HookEnabled = 1;
                 }
              }
              break;

           case SWM_DISABLE:
              if (HookEnabled == 1) {
                 releasehook();
                 HookEnabled = 0;
              }
              break;

           case SWM_EXIT:
              DestroyWindow(hWnd);
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

   if (strcasecmp(lpCmdLine, "/m") == 0) {
      write_default_map();
      sprintf(my_msg,
                "KeyTrans © 2005 - Suriyan Ramasami.\n"
                "VUTAM Font © 2005 - V. Ramasami.\n"
                VERSION
                "\n\n"
                "Sample INI file generated in %s"
                , INIFILE
             );
      MessageBox(NULL, my_msg, "KeyTrans", MB_OK);
      return(1);
   }
   else if ( (strcasecmp(lpCmdLine, "/?") == 0) || 
             (strcasecmp(lpCmdLine, "/h") == 0) ) {
      // We give the help syntax.
      sprintf(my_msg, 
                "KeyTrans © 2005 - Suriyan Ramasami.\n"
                "VUTAM Font © 2005 - V. Ramasami.\n"
                VERSION
                "\n"
                "KeyTrans /m\t:Generate the default INI file tailored for VUTAM.\n"
                "KeyTrans /h\t:Help\n\n"
                "Initialization file:\n"
                "\t[Mappings]\n"
                "\tVirtual Key Name=Numeric Key Pad Keys punched.\n"
                "\t...\n\n"
                "\t[Ligatures]\n"
                "\tRule1=BUCKET<1|2|3> VK_KEY1 BUCKET<1|2|3> VK_KEY2 KeyCode\n"
             );
      MessageBox(NULL, my_msg, "KeyTrans", MB_OK);
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
   wcx.lpszClassName = "KeyTrans";
   wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
   if (!RegisterClassEx(&wcx)) {
      MessageBox(NULL, "Failed registering window class","KeyTrans",MB_OK);
      return(-1);
   }

   hWnd = CreateWindow(
                "KeyTrans",
                "KeyTrans",
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
      MessageBox(NULL, "Failed opening window","KeyTrans",MB_OK);
      return(-1);
   }

   // Tray Bar Icon.
   ZeroMemory(&niData,sizeof(NOTIFYICONDATA));
   // niData.cbSize = sizeof(NOTIFYICONDATA);
   niData.cbSize = NOTIFYICONDATA_V1_SIZE;
   niData.uID = TRAYICONID;
   niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
   // niData.hIcon = LoadIcon (NULL, IDI_APPLICATION);
   niData.hIcon = LoadIcon (hInst, MAKEINTRESOURCE(IDI_MYICON));

   niData.hWnd = hWnd;
   niData.uCallbackMessage = TRAY_MSG;
   strcpy(niData.szTip, "KeyTrans - VUTAM");
   Shell_NotifyIcon(NIM_ADD, &niData);

   if(niData.hIcon && DestroyIcon(niData.hIcon)) {
      niData.hIcon = NULL;
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

   GetCursorPos(&pt);
   hMenu = CreatePopupMenu();

   if (hMenu) {
      if (HookEnabled == 1) {
         InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_DISABLE, "Disable");
      }
      else {
         InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_ENABLE, "Enable");
      }
      InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_EXIT, "Exit");

      SetForegroundWindow(hWnd);
      TrackPopupMenu(hMenu, TPM_BOTTOMALIGN,
                     pt.x, pt.y, 0, hWnd, NULL );
      DestroyMenu(hMenu);
   }
}
