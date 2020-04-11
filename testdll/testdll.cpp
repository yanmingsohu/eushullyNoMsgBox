#include <iostream>
#include <Windows.h>

int main() {
  LoadLibrary("hookMsgbox32.dll");
  MessageBoxA(0, "Don't be display!", "Bad", MB_OK);
  std::cout << "ok\n";
  return 0;
}
