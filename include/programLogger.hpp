#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <print>



/**
 *  The color sequences are composed of sequences of numbers separated
       by semicolons. The most common codes are:
          ┌────┬───────────────────────┐
          │    │                       │
          │ 0  │ to restore default    │
          │    │ color                 │
          ├────┼───────────────────────┤
          │    │                       │
          │ 1  │ for brighter colors   │
          ├────┼───────────────────────┤
          │    │                       │
          │ 4  │ for underlined text   │
          ├────┼───────────────────────┤
          │    │                       │
          │ 5  │ for flashing text     │
          ├────┼───────────────────────┤-----
          │    │                       │foreground/text color
          │ 30 │ for black foreground  │
          ├────┼───────────────────────┤
          │    │                       │
          │ 31 │ for red foreground    │
          ├────┼───────────────────────┤
          │    │                       │
          │ 32 │ for green foreground  │
          ├────┼───────────────────────┤
          │    │                       │
          │ 33 │ for yellow (or brown) │
          │    │ foreground            │
          ├────┼───────────────────────┤
          │    │                       │
          │ 34 │ for blue foreground   │
          ├────┼───────────────────────┤
          │    │                       │
          │ 35 │ for purple foreground │
          ├────┼───────────────────────┤
          │    │                       │
          │ 36 │ for cyan foreground   │
          ├────┼───────────────────────┤
          │    │                       │
          │ 37 │ for white (or gray)   │
          │    │ foreground            │
          ├────┼───────────────────────┤-------------
          │    │                       │background
          │ 40 │ for black background  │
          ├────┼───────────────────────┤
          │    │                       │
          │ 41 │ for red background    │
          ├────┼───────────────────────┤
          │    │                       │
          │ 42 │ for green background  │
          ├────┼───────────────────────┤
          │    │                       │
          │ 43 │ for yellow (or brown) │
          │    │ background            │
          ├────┼───────────────────────┤
          │    │                       │
          │ 44 │ for blue background   │
          ├────┼───────────────────────┤
          │    │                       │
          │ 45 │ for purple background │
          ├────┼───────────────────────┤
          │    │                       │
          │ 46 │ for cyan background   │
          ├────┼───────────────────────┤
          │    │                       │
          │ 47 │ for white (or gray)   │
          │    │ background            │
          └────┴───────────────────────┘

 */


/*
Usage: 
using ProgramLogger::log;
using ProgramLogger::LogLevel;
Then call log function like so:
log(<message>);
or
log(<message>, LogLevel::<logLevel>);
*/
namespace ProgramLogger {
    static const bool printDebug = false;

	enum class LogLevel {
		INFO,
		WARNING,
		ERROR,
        DEBUG
	};

    struct ConsoleColors {
        const std::string reset = "\033[0m";
        const std::string red = "\033[31m";
        const std::string yellow = "\033[33m";
        const std::string white = "\033[37m";
        const std::string green = "\033[32m";
    };

    inline std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm;
        localtime_s(&now_tm, &now_time);

        std::ostringstream oss;
        oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    inline void log(const std::string& message, LogLevel level = LogLevel::INFO) {
        static ConsoleColors consoleColors; // <-- static local, not global
        static const bool printDebug = false;
        std::string timestamp = getCurrentTimestamp();
        switch (level) {
            case LogLevel::INFO:
                std::println("{}[{}][INFO]: {}{}", consoleColors.white, timestamp, message, consoleColors.reset);
                break;
            case LogLevel::WARNING:
                std::println("{}[{}][WARNING]: {}{}", consoleColors.yellow, timestamp, message, consoleColors.reset);
                break;
            case LogLevel::ERROR:
                std::print(std::cerr, "{}[{}][ERROR]: {}{}\n", consoleColors.red, timestamp, message, consoleColors.reset);
                break;
            case LogLevel::DEBUG:
                if (!printDebug) break;
                std::println("{}[{}][DEBUG]: {}{}", consoleColors.green, timestamp, message, consoleColors.reset);
                break;
        }
    }
}