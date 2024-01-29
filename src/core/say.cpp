module;
#include <fmt/core.h>
#include <fmt/color.h>
#include <string_view>

export module core.say;

void _log(const fmt::color color, const std::string_view message)
{
  switch (color)
  {
  case fmt::color::red:
    fmt::print(fmt::emphasis::bold | fg(color), "[ERROR]: {}\n", message);
    break;
  case fmt::color::yellow:
    fmt::print(fmt::emphasis::bold | fg(color), "[DEBUG]: {}\n", message);
    break;
  case fmt::color::royal_blue:
    fmt::print(fmt::emphasis::bold | fg(color), "[INFO]: {}\n", message);
    break;
  case fmt::color::orange:
    fmt::print(fmt::emphasis::bold | fg(color), "[WARN]: {}\n", message);
    break;

  default:
    fmt::print(fmt::emphasis::bold | fg(color), "[UNKNOWN]: {}\n", message);
    break;
  }
}
export namespace core::say {

void debug(std::string_view message)
{
#ifndef NDEBUG
  _log(fmt::color::yellow, message);
#endif// NDEBUG
}
void info(std::string_view message) { _log(fmt::color::royal_blue, message); }
void warn(std::string_view message) { _log(fmt::color::orange, message); }
void error(std::string_view message) { _log(fmt::color::red, message); }
}// namespace core::say

export namespace fmt {
using ::fmt::format;
using ::fmt::print;
using ::fmt::println;
}// namespace fmt
