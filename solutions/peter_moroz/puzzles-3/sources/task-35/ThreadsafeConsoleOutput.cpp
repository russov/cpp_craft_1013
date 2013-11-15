#include "ThreadsafeConsoleOutput.h"
#include <iostream>
#include <boost/thread.hpp>

namespace threadsafe_console_output
{

static boost::mutex cout_guard;

void PrintText(const std::string& text) {
  boost::mutex::scoped_lock lock(cout_guard);
  std::cout << text << std::endl;
}

} // namespace ThreadsafeConsoleOutput
