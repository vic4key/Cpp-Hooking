# Cpp Hooking (C++17 or later required)

With this library, you can set up function hooking easily and writing less code.

It supports both `Inline hooking` & `IAT hooking` on both 32-bit & 64-bit.

### Demo

Eg. To hook/un-hook a function with `Inline Hooking` technique, you only need to write codes as the following

```cpp
#include "cpp-hooking/hooking.h"

// Define the hooking function
int WINAPI hkMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
  lpText = L"INL Hooked";
  return INLHookingManager::instance().invoke<int>(MessageBoxW, hWnd, lpText, lpCaption, uType);
}

// Perform hooking
INLHookingManager::instance().hook(MessageBoxW, hkMessageBoxW);

// Perform un-hooking
INLHookingManager::instance().unhook(MessageBoxW);
```

Eg. To hook/un-hook a function with `IAT Hooking` technique, you only need to write codes as the following

```cpp
#include "cpp-hooking/hooking.h"

// Define the hooking entry
#define Entry_MessageBoxW { "cpp-hooking.exe"s, "user32.dll"s, "MessageBoxW"s }

// Define the hooking function
int WINAPI hkMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
  lpText = L"IAT Hooked";
  return IATHookingManager::instance().invoke<int>(Entry_MessageBoxW, hWnd, lpText, lpCaption, uType);
}

// Perform hooking
IATHookingManager::instance().hook(Entry_MessageBoxW, hkMessageBoxW);

// Perform un-hooking
IATHookingManager::instance().unhook(Entry_MessageBoxW);
```

## Installation

1. Installed [Vutils](https://github.com/vic4key/Vutils.git) library
2. Copy the [cpp-hooking](cpp-hooking) folder to your solution then `#include "cpp-hooking\cpp-hooking.h"`
3. Enjoy

## ToDo

- [ ] Merge to [Vutils](https://github.com/vic4key/Vutils.git) library

## Contact
Feel free to contact via [Twitter](https://twitter.com/vic4key) / [Gmail](mailto:vic4key@gmail.com) / [Blog](https://blog.vic.onl/) / [Website](https://vic.onl/)
