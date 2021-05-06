#include <log/log.h>

#include <chrono>
#include <iostream>
#include <filesystem>

namespace logging
{
namespace fs = std::filesystem;

std::string begin = "\033[38;5;";
std::string end = "\033[0m";
std::string red = begin + "160m";
std::string orange = begin + "202m";
std::string light_grey = begin + "252m";
std::string dark_grey = begin + "240m";
std::string green = begin + "70m";
std::string blue = begin + "75m";

void time()
{
    using sc = std::chrono::system_clock;
    auto t = sc::now();
    auto ms = duration_cast<std::chrono::milliseconds>(t.time_since_epoch()) % 1000;
    const std::time_t t_c = sc::to_time_t(t);
    std::cout << blue << "<"
              << std::put_time(std::localtime(&t_c), "%H:%M:%S")
              << "." << ms.count() << ">";
}

void location(sl loc)
{
    std::cout << green << fs::path{loc.file_name()}.filename().native()
              << "[" << loc.function_name() << "]"
              << ":" << loc.line() << " ";
}

void debug(const std::string& msg, const std::string& cat, sl loc)
{
    time();
    std::cout << " ";
    location(loc);
    std::cout << dark_grey << msg << end << std::endl;
}

void info(const std::string& msg, const std::string& cat, sl loc)
{
    time();
    std::cout << " ";
    location(loc);
    std::cout << light_grey << msg << end << std::endl;
}

void warn(const std::string& msg, const std::string& cat, sl loc)
{
    time();
    std::cout << " ";
    location(loc);
    std::cout << orange << msg << end << std::endl;
}

void error(const std::string& msg, const std::string& cat, sl loc)
{
    time();
    std::cout << " ";
    location(loc);
    std::cout << red << msg << end << std::endl;
}

void fail(const std::string& msg, const std::string& cat, sl loc)
{
    error(msg, cat, loc);
    std::terminate();
}

} // log
