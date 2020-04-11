#include <windows.h>
#include <stdio.h>
#include "detours.h"

#pragma comment(lib, "../bin/detours.lib")

static int (WINAPI* RealMessageBox)(HWND, LPCSTR, LPCSTR, UINT);


int WINAPI MyMessageBoxA(HWND hWnd, char* lpText, char* lpCaption, UINT uType) {
  return IDYES;
}


// must do it
__declspec(dllexport) void dummy() {}


BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved) {
  LONG error;

  if (DetourIsHelperProcess()) {
    return TRUE;
  }

  switch (dwReason) {
    case DLL_PROCESS_ATTACH:
      RealMessageBox = MessageBoxA;
      DetourRestoreAfterWith();

      LoadLibrary("user32.dll");
      DetourTransactionBegin();
      DetourUpdateThread(GetCurrentThread());

      DetourAttach(&(PVOID&)RealMessageBox, MyMessageBoxA);
      error = DetourTransactionCommit();
      break;

    case DLL_PROCESS_DETACH:
      DetourTransactionBegin();
      DetourUpdateThread(GetCurrentThread());

      DetourDetach(&(PVOID&)RealMessageBox, MyMessageBoxA);
      error = DetourTransactionCommit();
      break;
  }

  return TRUE;
}

//
///////////////////////////////////////////////////////////////// End of File.
