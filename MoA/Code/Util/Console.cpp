/*
 * Console window for Windows by
 * http://www.williamwilling.com/blog/?p=74
 */

#include "Console.h"
#include <cstdio>
#include <iostream>
#include <windows.h>

Console::Console()
{
  // create a console window
  BOOL success = AllocConsole();

  // redirect std::cout to our console window
  m_old_cout = std::cout.rdbuf();
  m_out.open("CONOUT$");
  std::cout.rdbuf(m_out.rdbuf());

  // redirect std::cerr to our console window
  m_old_cerr = std::cerr.rdbuf();
  m_err.open("CONOUT$");
  std::cerr.rdbuf(m_err.rdbuf());

  // redirect std::cin to our console window
  m_old_cin = std::cin.rdbuf();
  m_in.open("CONIN$");
  std::cin.rdbuf(m_in.rdbuf());
}

Console::~Console()
{
  // reset the standard streams
  std::cin.rdbuf(m_old_cin);
  std::cerr.rdbuf(m_old_cerr);
  std::cout.rdbuf(m_old_cout);

  // remove the console window
  FreeConsole();
}
