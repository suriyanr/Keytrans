#include <windows.h>
#include <objbase.h>
#include <shlobj.h>

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

// Below are the files as arrays.
#include "PackKeyTrans.h"
#include "PackKeyTransHookDll.h"
#include "PackVutamTTF.h"
#include "PackReadme.h"

/* File sizes for the below can be got from Installer.h */
#include "Installer.h"

#define NAME_KEYTRANS        "KeyTrans2.exe"
#define NAME_KEYTRANSHOOKDLL "KeyTransHook2.dll"
#define NAME_VUTAMTTF        "Vutam2.ttf"
#define NAME_README          "README.txt"

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

// Spawn the Explorer window at end, so user can install the Vutam Font. */
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
      MessageBox(NULL, "An Install is already in progress. Aborting Install.", "KeyTrans2 Installation", MB_OK);
      exit(-1);
   }

   if (GetProgramFilesPath(programfile, 1024) == 0) {
      MessageBox(NULL, "Couldnt obtain Program Files Directory path. - Aborting Install.", "KeyTrans2 Installation", MB_OK);

      /* Destroy the Semaphore. */
      CloseHandle(OnlyOneSem);

      exit(-1);
   }

   strcat(programfile, "\\KeyTrans2");
#ifdef DEBUG
   fprintf(stdout, "Program Files Path: %s\n", programfile);
#endif
   /* We have programfile as: "C:\Program Files" */

   /* Install it and quit. */
   if (InstallFiles(programfile) == 1) {
#ifdef DEBUG
      fprintf(stdout, "Installed KeyTrans2.\n");
#endif

      ShowInstallDir(programfile);
      MessageBox(NULL, "KeyTrans2 Installed successfully. Please install the Vutam2 Font.", "KeyTrans2 Installation", MB_OK);
   }
   else {
      MessageBox(NULL, "KeyTrans2 Installation Failed.", "KeyTrans2 Installation", MB_OK);

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

   fd = open(filename, O_WRONLY | O_CREAT | O_BINARY, S_IRUSR | S_IWUSR);
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

   OnlyOneSem = CreateSemaphore(NULL, 0, 1, "Vutam");
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
   strcat(desktop, "\\Vutam2.lnk"); // Link name on DeskTop

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_KEYTRANS);
   retval = MyCreateFile(fullfilepath, KeyTrans2, LENGTH_KEYTRANS);

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_KEYTRANSHOOKDLL);
   retval += MyCreateFile(fullfilepath, KeyTransHook2, LENGTH_KEYTRANSHOOKDLL);

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_VUTAMTTF);
   retval += MyCreateFile(fullfilepath, Vutam2, LENGTH_VUTAMTTF);

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_README);
   retval += MyCreateFile(fullfilepath, README, LENGTH_README);

   sprintf(fullfilepath, "%s\\%s", programpath, NAME_KEYTRANS);
   MakeDesktopLink(desktop, fullfilepath);

   if (retval == 4) return(1);
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

