#pragma once

#include "common.h"

#include <vu>

class INLHookingManager : public vu::SingletonT<INLHookingManager>
{
  struct inl_hooked_t: public hooked_t
  {
    vu::INLHooking m_hooker;

    inl_hooked_t() : hooked_t() {}

    inl_hooked_t(const inl_hooked_t& right)
    {
      *this = right;
    }

    const inl_hooked_t& operator=(const inl_hooked_t& right)
    {
      if (this != &right)
      {
        hooked_t::operator=(right);
        m_hooker = right.m_hooker;
      }

      return *this;
    }
  };

  std::unordered_map<void*, inl_hooked_t> m_list;

public:
  template<typename Function>
  bool hook(void* function, Function&& hk_function)
  {
    auto it = m_list.find(function);
    if (it != m_list.cend())
    {
      return false;
    }

    inl_hooked_t hooked;
    if (!hooked.m_hooker.attach(function, (void*)hk_function, &hooked.m_trampoline))
    {
      return false;
    }

    using FunctionPtr = decltype(&hk_function);

    hooked.m_function = function;
    hooked.m_invoker.reset(new Invokable(FunctionPtr(hooked.m_trampoline)));

    void* key = hooked.m_function;
    m_list[key] = std::move(hooked);

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

  template<typename StdString, typename Function>
  bool hook(const StdString& module, const StdString& function, Function&& hk_function)
  {
    auto ptr = get_proc_address<StdString>(module, function);
    return ptr != nullptr ? this->hook(ptr, hk_function) : false;
  }

  template<typename StdString>
  bool unhook(const StdString& module, const StdString& function)
  {
    auto ptr = get_proc_address<StdString>(module, function);
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

    auto& hooked = it->second;

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
};
