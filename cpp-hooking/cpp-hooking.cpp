// cpp-hooking.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "cpp-hooking/hooking.h"

int WINAPI hkMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
  lpText = "Hooked";
  return INLHookingManager::instance().invoke<int>(MessageBoxA, hWnd, lpText, lpCaption, uType);
}

int WINAPI hkMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
  lpText = L"Hooked";
  return INLHookingManager::instance().invoke<int>(MessageBoxW, hWnd, lpText, lpCaption, uType);
}

int main()
{
  INLHookingManager::instance().hook(MessageBoxA, hkMessageBoxA);
  INLHookingManager::instance().hook(MessageBoxW, hkMessageBoxW);
  MessageBoxA(nullptr,  "The first message.",  "MessageBoxA", MB_OK); // text is replaced by "Hooked"
  MessageBoxW(nullptr, L"The first message.", L"MessageBoxW", MB_OK); // text is replaced by "Hooked"

  INLHookingManager::instance().unhook(MessageBoxA);
  INLHookingManager::instance().unhook(MessageBoxW);
  MessageBoxA(nullptr,  "The second message.",  "MessageBoxA", MB_OK);
  MessageBoxW(nullptr, L"The second message.", L"MessageBoxW", MB_OK);

  return 0;
}
