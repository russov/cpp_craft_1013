#ifndef THREADSAFE_CONSOLE_OUTPUT_H_
#define THREADSAFE_CONSOLE_OUTPUT_H_

#include <string>

namespace threadsafe_console_output
{

// threadsafe method, for exclusive print text message to console
void PrintText(const std::string& text);

}

#endif // THREADSAFE_CONSOLE_OUTPUT_H_