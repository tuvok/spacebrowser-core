#ifndef LOG_H
#define LOG_H

#include <experimental/source_location>
#include <string>

namespace logging
{
using sl = std::experimental::source_location;
void debug(const std::string& msg, const std::string& cat = "", sl loc = sl::current());
void info (const std::string& msg, const std::string& cat = "", sl loc = sl::current());
void warn (const std::string& msg, const std::string& cat = "", sl loc = sl::current());
void error(const std::string& msg, const std::string& cat = "", sl loc = sl::current());
void fail (const std::string& msg, const std::string& cat = "", sl loc = sl::current());
}

#endif // LOG_H
