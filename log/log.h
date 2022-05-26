#ifndef LOG_H
#define LOG_H

#include <experimental/source_location>
#include <string>

namespace logging
{
using sl = std::experimental::source_location;

/**
 * Simple functions to perform stdout logging
 *
 * debug() prints messages in dark grey
 * info() prints messages in light grey
 * warn() prints messages in orange
 * error() prints messages in red
 * fail() prints messages in red and calls std::terminate()
 *
 * @param msg message to be logged
 * @param cat logging category (currently unused)
 * @param loc source code location
 */
void debug(const std::string& msg, const std::string& cat = "", sl loc = sl::current());
void info (const std::string& msg, const std::string& cat = "", sl loc = sl::current());
void warn (const std::string& msg, const std::string& cat = "", sl loc = sl::current());
void error(const std::string& msg, const std::string& cat = "", sl loc = sl::current());
void fail (const std::string& msg, const std::string& cat = "", sl loc = sl::current());
}

#endif // LOG_H
