#pragma once

#include "invokable.h"

#include <memory>

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
