// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "cpp-hooking/hooking.h"

// Inline Hooking

int WINAPI hkMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
  lpText = "INL Hooked";
  return INLHookingManager::instance().invoke<int>(MessageBoxA, hWnd, lpText, lpCaption, uType);
}

int WINAPI hkMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
  lpText = L"INL Hooked";
  return INLHookingManager::instance().invoke<int>(MessageBoxW, hWnd, lpText, lpCaption, uType);
}

// IAT Hooking

#include <string>
using namespace std::literals::string_literals;

#define Entry_MessageBoxA { "cpp-hooking.exe"s, "user32.dll"s, "MessageBoxA"s }
#define Entry_MessageBoxW { "cpp-hooking.exe"s, "user32.dll"s, "MessageBoxW"s }

int WINAPI iat_hkMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
  lpText = "IAT Hooked";
  return IATHookingManager::instance().invoke<int>(Entry_MessageBoxA, hWnd, lpText, lpCaption, uType);
}

int WINAPI iat_hkMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
  lpText = L"IAT Hooked";
  return IATHookingManager::instance().invoke<int>(Entry_MessageBoxW, hWnd, lpText, lpCaption, uType);
}

int main()
{
  // Inline Hooking

  INLHookingManager::instance().hook(MessageBoxA, hkMessageBoxA);
  INLHookingManager::instance().hook(MessageBoxW, hkMessageBoxW);
  MessageBoxA(nullptr,  "The first message.",  "MessageBoxA", MB_OK); // text is replaced by "INL Hooked"
  MessageBoxW(nullptr, L"The first message.", L"MessageBoxW", MB_OK); // text is replaced by "INL Hooked"

  INLHookingManager::instance().unhook(MessageBoxA);
  INLHookingManager::instance().unhook(MessageBoxW);
  MessageBoxA(nullptr,  "The second message.",  "MessageBoxA", MB_OK);
  MessageBoxW(nullptr, L"The second message.", L"MessageBoxW", MB_OK);

  // IAT Hooking

  IATHookingManager::instance().hook(Entry_MessageBoxA, iat_hkMessageBoxA);
  IATHookingManager::instance().hook(Entry_MessageBoxW, iat_hkMessageBoxW);
  MessageBoxA(nullptr,  "The first message.",  "MessageBoxA", MB_OK); // text is replaced by "IAT Hooked"
  MessageBoxW(nullptr, L"The first message.", L"MessageBoxW", MB_OK); // text is replaced by "IAT Hooked"

  IATHookingManager::instance().unhook(Entry_MessageBoxA);
  IATHookingManager::instance().unhook(Entry_MessageBoxW);
  MessageBoxA(nullptr,  "The second message.",  "MessageBoxA", MB_OK);
  MessageBoxW(nullptr, L"The second message.", L"MessageBoxW", MB_OK);

  return 0;
}
