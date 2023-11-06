#pragma once

#include "common.h"

#include <vu>

class IATHookingManager : public vu::SingletonT<IATHookingManager>
{
  struct iat_hooked_t : public hooked_t {};

  std::unordered_map<void*, iat_hooked_t> m_list;

  using Entry = vu::IATHookingA::Entry;

  vu::IATHookingA& hooker()
  {
    return vu::IATHookingA::instance();
  }

  void* get_proc_address(const Entry& entry)
  {
    Entry temp;

    if (!this->hooker().exist(entry.target, entry.module, entry.function, &temp))
    {
      return nullptr;
    }

    return temp.original;
  }

public:
  template<typename Function>
  bool hook(const Entry& entry, Function&& hk_function)
  {
    iat_hooked_t hooked;

    auto ret = this->hooker().install(entry.target, entry.module, entry.function,
      (void*)hk_function, &hooked.m_trampoline);
    if (ret != vu::VU_OK)
    {
      return false;
    }

    auto function = this->get_proc_address(entry);
    if (function == nullptr)
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

  bool unhook(const Entry& entry)
  {
    auto function = this->get_proc_address(entry);
    if (function == nullptr)
    {
      return false;
    }

    auto it = m_list.find(function);
    if (it == m_list.cend())
    {
      return false;
    }

    auto ret = this->hooker().uninstall(entry.target, entry.module, entry.function);
    if (ret != vu::VU_OK)
    {
      return false;
    }

    m_list.erase(it);

    return true;
  }

  template<typename Return, typename ... Args>
  Return invoke(const Entry& entry, Args ... args)
  {
    auto function = this->get_proc_address(entry);
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
  }
};
