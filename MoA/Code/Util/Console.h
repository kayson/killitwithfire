/*
 * Console window for Windows by
 * http://www.williamwilling.com/blog/?p=74
 */

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <fstream>

class Console
{
private:
  std::ofstream m_out;
  std::ofstream m_err;
  std::ifstream m_in;

  std::streambuf* m_old_cout;
  std::streambuf* m_old_cerr;
  std::streambuf* m_old_cin;

public:
  Console();
  ~Console();
};

#endif
