#include <windows.h>
#include <objbase.h>
#include <shlobj.h>

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

// Below are the files as arrays.
// EXE and DLL
#include "PackKeyTransU.h"
#include "PackKeyTransUDll.h"

// TXT files
#include "PackTam_Maps.h"
#include "PackReadme.h"

// TTF files
#include "PackVutam2TTF.h"
#include "PackVutam3TTF.h"

// BMP files
#include "PackVutam2SpecialBMP.h"

// The 5 map files.
#include "PackKeyTransUMap.h"
#include "PackEngTranslitMap.h"
#include "PackTACEphonMap.h"
#include "PackTamNet99Map.h"
#include "PackTamPhonMap.h"

#define NAME_KEYTRANSU       "KeyTransU.exe"
#define NAME_KEYTRANSUDLL    "KeyTransU.dll"
#define NAME_README          "README.txt"
#define NAME_TAM_MAPS        "Tam_Maps.txt"
#define NAME_VUTAM2TTF       "Vutam2.ttf"
#define NAME_VUTAM3TTF       "Vutam3.ttf"
#define NAME_VUTAM2SPECIALBMP "Vutam2Special.bmp"
#define NAME_ENGTRANSLITMAP  "KeyTransU_EngTranslit.map"
#define NAME_KEYTRANSUMAP    "KeyTransU.map"
#define NAME_TACEPHONEMAP    "KeyTransU_TACEphon.map"
#define NAME_TAMNET99MAP     "KeyTransU_TamNet99.map"
#define NAME_TAMPHONMAP      "KeyTransU_TamPhon.map"

/* global semaphore so only one instance can run. */
HANDLE OnlyOneSem = NULL;

/* Returns 1 if already running, else returns 0. */
int CheckIfRunning();

/* Returns 1 on success, Returns 0 on error */
int MyCreateFile(char *filename, const unsigned char *buffer, size_t size);

/* Returns 1 on success. 0 on failure. str is of size sz,  */
/* and is allocated by caller. */
int GetProgramFilesPath(char *str, int sz);

/* Returns 1 on success. 0 on failure. str is of size sz,  */
/* and is allocated by caller. */
int GetDesktopPath(char *str, int sz);

/* Trys to copy the files in the proper places. */
/* Returns 1 on success, 0 on failure */
int InstallFiles(char *programpath);

/* Create the install directpry */
/* Returns 1 on success. else 0 for failure. */
int CreateInstallDirectory(char *programpath);

/* Make the desktop link. */
/* Returns 1 on success. 0 on error. */
int MakeDesktopLink(char *desktop, char *program);

// Spawn the Explorer window at end, so user can install the Utam Font. */
/* Returns 1 on success. 0 on error. */
int ShowInstallDir(char *installpath);

int main() {
PROCESS_INFORMATION piProcInfo;
STARTUPINFO siStartInfo;
char cmdline[1024];
char programfile[1024];

   /* Only one invocation allowed. */
   /* check if we are already running. */
   if (CheckIfRunning()) {
      MessageBox(NULL, "An Install is already in progress. Aborting Install.", "KeyTransU Installation", MB_OK);
      exit(-1);
   }

   if (GetProgramFilesPath(programfile, 1024) == 0) {
      MessageBox(NULL, "Couldnt obtain Program Files Directory path. - Aborting Install.", "KeyTransU Installation", MB_OK);

      /* Destroy the Semaphore. */
      CloseHandle(OnlyOneSem);

      exit(-1);
   }

   strcat(programfile, "\\KeyTransU");
#ifdef DEBUG
   fprintf(stdout, "Program Files Path: %s\n", programfile);
#endif
   /* We have programfile as: "C:\Program Files" */

   /* Install it and quit. */
   if (InstallFiles(programfile) == 1) {
#ifdef DEBUG
      fprintf(stdout, "Installed KeyTransU.\n");
#endif

      MessageBox(NULL, "KeyTransU Installed successfully.\n\nOnce you press OK, the Installation folder shall be opened.\nOn Vista, please right click on UTAM.ttf and choose the install option.\nFor XP/2000, open Control Panel -> Fonts and install the UTAM font", "KeyTransU Installation", MB_OK);
      ShowInstallDir(programfile);
   }
   else {
      MessageBox(NULL, "KeyTransU Installation Failed.", "KeyTransU Installation", MB_OK);

      /* Destroy the Semaphore. */
      CloseHandle(OnlyOneSem);

      exit(-1);
   }

   /* Destroy the Semaphore. */
   CloseHandle(OnlyOneSem);

   /* and we exit. */
   exit(0);
}

/* Returns 1 on success, Returns 0 on error */
int MyCreateFile(char *filename, const unsigned char *buffer, size_t size) {
int fd;
int retval;

#ifdef DEBUG
   fprintf(stdout, "In MyCreateFile() filename: %s\n", filename);
#endif

   fd = open(filename, O_WRONLY | O_CREAT | O_BINARY, S_IRUSR | S_IWUSR);
#ifdef DEBUG
   fprintf(stdout, "open returned %d\n", fd);
#endif

   if (fd == -1) return(0);

   retval = write(fd, buffer, size);
   close(fd);

   if (retval != size) return(0);

   return(1);
}

void MyDeleteFile(char *filename) {

   unlink(filename);
}

void HideFile(char *filename) {

   SetFileAttributes(filename, FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
}

/* Returns 1 if already running, else returns 0. */
int CheckIfRunning() {

   OnlyOneSem = CreateSemaphore(NULL, 0, 1, "Utam");
   if (OnlyOneSem == NULL) {
      // Semaphore already exists.
      return(1);
   }

   if (GetLastError() == ERROR_ALREADY_EXISTS) {
      // Semaphore already exits.
      return(1);
   }
   return(0);
}

/* Returns 1 on success. 0 on failure. str is of size sz,  */
/* and is allocated by caller. */
int GetDesktopPath(char *str, int sz) {
HKEY hkey;
DWORD mysz = sz;

   if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                    "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
                    0,
                    KEY_QUERY_VALUE,
                    &hkey) != ERROR_SUCCESS) {
      return(0);
   }

   if (RegQueryValueEx(hkey,
                       "Common Desktop",
                       NULL,
                       NULL,
                       str,
                       &mysz) != ERROR_SUCCESS) {
      RegCloseKey(hkey);
      return(0);
   }

   RegCloseKey(hkey);
   return(1);
}

/* Returns 1 on success. 0 on failure. str is of size sz,  */
/* and is allocated by caller. */
int GetProgramFilesPath(char *str, int sz) {
HKEY hkey;
DWORD mysz = sz;

#if 0
   if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                    "SOFTWARE\\Microsoft\\Windows\\CurrentVersion",
                    0,
                    KEY_QUERY_VALUE,
                    &hkey) != ERROR_SUCCESS) {
      return(0);
   }

   if (RegQueryValueEx(hkey,
                       "ProgramFilesDir",
                       NULL,
                       NULL,
                       str,
                       &mysz) != ERROR_SUCCESS) {
      RegCloseKey(hkey);
      return(0);
   }
#endif

   if (RegOpenKeyEx(HKEY_CURRENT_USER,
                    "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
                    0,
                    KEY_QUERY_VALUE,
                    &hkey) != ERROR_SUCCESS) {
      return(0);
   }

   if (RegQueryValueEx(hkey,
                       "Personal",
                       NULL,
                       NULL,
                       str,
                       &mysz) != ERROR_SUCCESS) {
      RegCloseKey(hkey);
      return(0);
   }

   RegCloseKey(hkey);
   return(1);
}


/* Trys to copy the files in the proper places. */
/* Returns 1 on success, 0 on failure */
int InstallFiles(char *programpath) {
char fullfilepath[1024];
char desktop[1024];
int retval = 0;

   CreateInstallDirectory(programpath);

   if (GetDesktopPath(desktop, 1024) == 0) {
#ifdef DEBUG
      fprintf(stderr, "Couldnt obtain Desktop path.\n");
#endif
      desktop[0] = '\0';
   }
#ifdef DEBUG
   fprintf(stdout, "Desktop Path: %s\n", desktop);
#endif
   strcat(desktop, "\\Vutam3.lnk"); // Link name on DeskTop

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_KEYTRANSU);
   retval = MyCreateFile(fullfilepath, PackKeyTransU, sizeof(PackKeyTransU));

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_KEYTRANSUDLL);
   retval += MyCreateFile(fullfilepath, PackKeyTransUDll, sizeof(PackKeyTransUDll));

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_README);
   retval += MyCreateFile(fullfilepath, PackReadme, sizeof(PackReadme));

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_TAM_MAPS);
   retval += MyCreateFile(fullfilepath, PackTam_Maps, sizeof(PackTam_Maps));

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_VUTAM2TTF);
   retval += MyCreateFile(fullfilepath, PackVutam2TTF, sizeof(PackVutam2TTF));

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_VUTAM3TTF);
   retval += MyCreateFile(fullfilepath, PackVutam3TTF, sizeof(PackVutam3TTF));

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_VUTAM2SPECIALBMP);
   retval += MyCreateFile(fullfilepath, PackVutam2SpecialBMP, sizeof(PackVutam2SpecialBMP));

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_ENGTRANSLITMAP);
   retval += MyCreateFile(fullfilepath, PackEngTranslitMap, sizeof(PackEngTranslitMap));

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_KEYTRANSUMAP);
   retval += MyCreateFile(fullfilepath, PackKeyTransUMap, sizeof(PackKeyTransUMap));

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_TACEPHONEMAP);
   retval += MyCreateFile(fullfilepath, PackTACEphonMap, sizeof(PackTACEphonMap));

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_TAMNET99MAP);
   retval += MyCreateFile(fullfilepath, PackTamNet99Map, sizeof(PackTamNet99Map));

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_TAMPHONMAP);
   retval += MyCreateFile(fullfilepath, PackTamPhonMap, sizeof(PackTamPhonMap));

   // Create Desktop Link
   sprintf(fullfilepath, "%s\\%s", programpath, NAME_KEYTRANSU);
   MakeDesktopLink(desktop, fullfilepath);

   // We create 12 files above
   if (retval == 12) return(1);
   else return(0);
}

/* Create the install directpry */
/* Returns 1 on success. else 0 for failure. */
int CreateInstallDirectory(char *programpath) {

   if (mkdir(programpath) == -1) {
      return(0);
   }
   return(1);
}

// Spawn the Explorer window at end, so user can install the Vutam Font. */
/* Returns 1 on success. 0 on error. */
int ShowInstallDir(char *installpath) {
STARTUPINFO si;
PROCESS_INFORMATION pi;
char arglist[MAX_PATH];

   ZeroMemory( &si, sizeof(si) );
   si.cb = sizeof(si);
   ZeroMemory( &pi, sizeof(pi) );

   sprintf(arglist, "explorer %s", installpath);

   if (CreateProcess(NULL, arglist,
        NULL,       // Process handle not inheritable.
        NULL,       // Thread handle not inheritable.
        FALSE,      // Set handle inheritance to TRUE to access server_pid
        0,          // No creation flags.
        NULL,       // Use parent's environment block.
        NULL,       // Use parent's starting directory.
        &si,        // Pointer to STARTUPINFO structure.
        &pi )       // Pointer to PROCESS_INFORMATION structure.
        ) {

        // Handles in PROCESS_INFORMATION must be closed with CloseHandle
        // when they are no longer needed.
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return(1);
   }

   return(0);
}

