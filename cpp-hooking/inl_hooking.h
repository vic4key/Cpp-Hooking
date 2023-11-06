#pragma once

#include "invokable.h"

#include <vu>
using namespace vu;

class INLHookingManager : public SingletonT<INLHookingManager>
{
  struct hooked_t
  {
    std::wstring m_module;
    void* m_function;
    void* m_trampoline;
    vu::INLHooking m_hooker;
    std::shared_ptr<Invokable> m_invoker;

    hooked_t()
      : m_module(L""), m_function(0), m_trampoline(0) {}

    hooked_t(void* function, const std::wstring& module = L"")
      : m_module(module), m_function(function), m_trampoline(0) {}

    hooked_t(const hooked_t& right)
    {
      *this = right;
    }

    const hooked_t& operator=(const hooked_t& right)
    {
      m_module = right.m_module;
      m_function = right.m_function;
      m_trampoline = right.m_trampoline;
      m_hooker = right.m_hooker;
      m_invoker = right.m_invoker;
      return *this;
    }
  };

  std::unordered_map<void*, hooked_t> m_list;

public:
  template<typename Function>
  bool hook(void* function, Function&& hk_function)
  {
    auto it = m_list.find(function);
    if (it != m_list.cend())
    {
      return false;
    }

    hooked_t hooked(function);
    if (!hooked.m_hooker.attach(hooked.m_function, (void*)hk_function, &hooked.m_trampoline))
    {
      return false;
    }

    using FunctionPtr = decltype(&hk_function);

    hooked.m_invoker.reset(new Invokable(FunctionPtr(hooked.m_trampoline)));
    m_list[function] = std::move(hooked);

    return true;
  }

  bool unhook(void* function)
  {
    auto it = m_list.find(function);
    if (it == m_list.cend())
    {
      return false;
    }

    auto& hooked = it->second;
    auto result = hooked.m_hooker.detach(function, &hooked.m_trampoline);
    if (result)
    {
      m_list.erase(it);
    }

    return result;
  }

  template<typename std_string, typename function_t>
  bool hook(const std_string& module, const std_string& function, function_t&& hk_function)
  {
    auto ptr = this->get_proc_address<std_string>(module, function);
    return ptr != nullptr ? this->hook(ptr, hk_function) : false;
  }

  template<typename std_string>
  bool unhook(const std_string& module, const std_string& function)
  {
    auto ptr = this->get_proc_address<std_string>(module, function);
    return ptr != nullptr ? this->unhook(ptr) : false;
  }

  template<typename Return, typename ... Args>
  Return invoke(void* function, Args ... args)
  {
    auto it = m_list.find(function);
    if (it == m_list.cend())
    {
      throw "invoke the function that did not hook";
    }

    auto& hooked = m_list[function];

    if (std::is_void<Return>::value)
    {
      hooked.m_invoker->invoke<Return>(std::forward<Args>(args)...);
    }
    else
    {
      return hooked.m_invoker->invoke<Return>(std::forward<Args>(args)...);
    }

    return 0;
  }

private:
  template <typename T>
  struct LibraryT;

  template <>
  struct LibraryT<std::string>
  {
    typedef LibraryA self;
  };

  template <>
  struct LibraryT<std::wstring>
  {
    typedef LibraryW self;
  };

  template<typename std_string>
  void* get_proc_address(const std_string& module, const std_string& function)
  {
    if (module.empty() || function.empty())
    {
      return nullptr;
    }

    return LibraryT<std_string>::self::quick_get_proc_address(module, function);
  }
};
