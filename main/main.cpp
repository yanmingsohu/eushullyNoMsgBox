#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include "detours.h"

#pragma comment(lib, "../bin/detours.lib")

#define HOOK_DLL_NAME   "hookMsgbox32.dll"
#define AGE_EXE_NAME    "AGE.EXE"
#define AGE_CN_NAME     "AGE_cn.exe"
#define TEST_EXE        "testdll.exe"
#define CN_TRANSLATION  "mai.dll"
#define FILE_FOUND      0


static bool file_exists(const char* filename) {
  return FILE_FOUND == _access(filename, _A_NORMAL);
}


int main() {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;
	memset(&si,0,sizeof(STARTUPINFO));
	memset(&pi,0,sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);

  if (!file_exists(HOOK_DLL_NAME)) {
    MessageBox(0, HOOK_DLL_NAME " not found", "Error", MB_OK);
    return 1;
  }

  const char* exe_file = 0;

  if (file_exists(AGE_CN_NAME)) {
    if (file_exists(AGE_EXE_NAME) && file_exists(CN_TRANSLATION)) {
      exe_file = AGE_EXE_NAME;
    } else {
      exe_file = AGE_CN_NAME;
    }
  }
  else if (file_exists(AGE_EXE_NAME)) {
    exe_file = AGE_EXE_NAME;
  }
  else {
    MessageBox(0, AGE_CN_NAME " or " AGE_EXE_NAME " not found", "Error", MB_OK);
    return 1;
  }

	if(DetourCreateProcessWithDllEx(
        exe_file,       // No module name (use command line)
		    0,              // Command line
		    NULL,           // Process handle not inheritable
		    NULL,           // Thread handle not inheritable
		    FALSE,          // Set handle inheritance to FALSE
		    0,              // No creation flags
		    NULL,           // Use parent's environment block
		    NULL,           // Use parent's starting directory 
		    &si,            // Pointer to STARTUPINFO structure
		    &pi ,				    // Pointer to PROCESS_INFORMATION structure
        HOOK_DLL_NAME,
		    NULL)) {
		printf("³É¹¦\n");
	} else {
    char msg[100];
    DWORD dwError = GetLastError();
    sprintf_s(msg, "CODE: %d", dwError);
    MessageBox(0, msg, "Error", MB_OK);
	}
	return 0;
}