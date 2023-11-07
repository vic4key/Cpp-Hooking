/**
 * @file   common.h
 * @author Vic P.
 * @brief  Header for common use.
 */

#pragma once

#include "invokable.h"

#include <memory>
#include <vu>

struct hooked_t
{
  void* m_function;
  void* m_trampoline;
  std::shared_ptr<Invokable> m_invoker;

  hooked_t() : m_function(nullptr), m_trampoline(nullptr) {}

  hooked_t(const hooked_t& right)
  {
    *this = right;
  }

  const hooked_t& operator=(const hooked_t& right)
  {
    if (this != &right)
    {
      m_function = right.m_function;
      m_trampoline = right.m_trampoline;
      m_invoker = right.m_invoker;
    }

    return *this;
  }
};

template <typename T>
struct LibraryT;

template <>
struct LibraryT<std::string>
{
  typedef vu::LibraryA self;
};

template <>
struct LibraryT<std::wstring>
{
  typedef vu::LibraryW self;
};

template<typename StdString>
inline void* get_proc_address(const StdString& module, const StdString& function)
{
  if (module.empty() || function.empty())
  {
    return nullptr;
  }

  return LibraryT<StdString>::self::quick_get_proc_address(module, function);
}
