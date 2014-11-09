#include <windows.h>
#include <objbase.h>
#include <shlobj.h>

/* Make the desktop link. */
/* Returns 1 on success. 0 on error. */
extern "C" int MakeDesktopLink(char *desktop, char *program) {
HRESULT hres;
IShellLink *psl;
int fn_len;

   if (CoInitialize(NULL) != S_OK) {
      return(0);
   }

   hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
                            IID_IShellLink, (void **)&psl);

   if (SUCCEEDED(hres)) 
    { 
        IPersistFile *ppf; 
 
        // Set the path to the shortcut target and add the description. 
        psl->SetPath(program);
        psl->SetDescription("KeyTransU / Utam Bundle");
        // Lets get the working directory.
        fn_len = strlen(program);
        while (fn_len > 0) {
           if (program[fn_len] == '\\') {
              program[fn_len] = '\0';
              break;
           }
           fn_len--;
        }
	psl->SetWorkingDirectory(program);
 
        // Query IShellLink for the IPersistFile interface for saving the 
        // shortcut in persistent storage. 
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); 
 
        if (SUCCEEDED(hres)) 
        { 
            WCHAR wsz[MAX_PATH]; 
 
            // Ensure that the string is Unicode. 
            MultiByteToWideChar(CP_ACP, 0, desktop, -1, wsz, MAX_PATH); 

            // Save the link by calling IPersistFile::Save. 
            hres = ppf->Save(wsz, TRUE); 
            ppf->Release(); 
        } 
        psl->Release(); 
    }
    CoUninitialize();
    return(1);
}
