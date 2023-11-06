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
  lpText = L"Hooked";
  return INLHookingManager::instance().invoke<int>(MessageBoxW, hWnd, lpText, lpCaption, uType);
}

// Perform hooking
INLHookingManager::instance().hook(MessageBoxW, hkMessageBoxW);

// Perform un-hooking
INLHookingManager::instance().unhook(MessageBoxW);
```

## Installation

1. Installed [Vutils](https://github.com/vic4key/Vutils.git) library
2. Copy the [cpp-hooking](cpp-hooking) folder to your solution then `#include "cpp-hooking\cpp-hooking.h"`
3. Enjoy

## ToDo

1. Implement `IAT Hooking Manager` class
2. Merge to [Vutils](https://github.com/vic4key/Vutils.git) library

## Contact
Feel free to contact via [Twitter](https://twitter.com/vic4key) / [Gmail](mailto:vic4key@gmail.com) / [Blog](https://blog.vic.onl/) / [Website](https://vic.onl/)
