#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <thread>
#include <mutex>
#include "Util/Formatter.hpp"

#define Logger_WriteConsole(message, level) __Logger::WriteConsole(message, std::string(__FUNCTION__), level)
#define Logger_ThrowError(unexpected, message, fatal) __Logger::ThrowError(unexpected, message, std::string(__FUNCTION__), __LINE__, fatal)

using namespace SurvivalCraft::Util;

namespace SurvivalCraft
{
    namespace Core
    {
        enum class LogLevel
        {
            INFORMATION,
            DEBUGGING,
            WARNING,
            ERROR,
            FATAL_ERROR
        };

        class __Logger
        {

        public:

            static void WriteConsole(const std::string& message, const std::string& function, const LogLevel& level)
            {
                std::string name = GetFunctionName(function);

                name[0] = std::toupper(name[0]);

                std::string time = GetDateTime("%H:%M:%S");

                std::thread::id threadId = std::this_thread::get_id();

                std::stringstream thread;
                thread << threadId;

                switch (level)
                {

                case LogLevel::INFORMATION:

                    std::cout << Formatter::ColorFormat(Formatter::Format("&2&l[{}] [Thread ({})/INFORMATION] [{}]: {}&r", time, thread.str(), name, message)) << std::endl;
                    break;

                case LogLevel::DEBUGGING:

                    std::cout << Formatter::ColorFormat(Formatter::Format("&1&l[{}] [Thread ({})/DEBUGGING] [{}]: {}&r", time, thread.str(), name, message)) << std::endl;
                    break;

                case LogLevel::WARNING:

                    std::cout << Formatter::ColorFormat(Formatter::Format("&e[{}] [Thread ({})/WARNING] [{}]: {}&r", time, thread.str(), name, message)) << std::endl;
                    break;

                case LogLevel::ERROR:

                    std::cout << Formatter::ColorFormat(Formatter::Format("&4&l[{}] [Thread ({})/ERROR] [{}]: {}&r", time, thread.str(), name, message)) << std::endl;
                    break;

                case LogLevel::FATAL_ERROR:

                    std::cout << Formatter::ColorFormat(Formatter::Format("&4[{}] [Thread ({})/FATAL ERROR] [{}]: {}&r", time, thread.str(), name, message)) << std::endl;
                    break;

                default:
                    break;
                }
            }

            static void ThrowError(const std::string& unexpected, const std::string& message, const std::string& function, int line, bool fatal)
			{
                if (fatal)
                {
                    __Logger::WriteConsole(Formatter::Format("Unexpected: '{}'\nAt: '{}::{}',\n'{}'.", unexpected, function, std::to_string(line), message), function, LogLevel::FATAL_ERROR);

                    exit(-1);
                }
                else
                    __Logger::WriteConsole(Formatter::Format("Unexpected: '{}' At: '{}::{}', '{}'.", unexpected, function, std::to_string(line), message), function, LogLevel::ERROR);
			}

        private:

            static std::string GetFunctionName(const std::string& name)
            {
                size_t position = name.rfind("::");

                if (position != std::string::npos)
                    return name.substr(position + 2);
                else
                    return name;
            }

            static std::string GetFullFunctionName(const std::string& name) 
            {
                size_t lastPosition = name.rfind("::");

                if (lastPosition != std::string::npos)
                {
                    size_t secondLastPosition = name.rfind("::", lastPosition - 1);

                    if (secondLastPosition != std::string::npos) 
                        return name.substr(0, secondLastPosition + 2) + name.substr(lastPosition + 2);
                    else 
                        return name;
                }
                else 
                    return name;
            }

            static std::string GetDateTime(const std::string& format)
            {
                std::chrono::system_clock::time_point rawNow = std::chrono::system_clock::now();
                std::time_t currentTime = std::chrono::system_clock::to_time_t(rawNow);

                std::tm timeNow;
                {
                    std::lock_guard<std::mutex> lock(guard);
                    timeNow = *std::localtime(&currentTime);
                }

                std::ostringstream buffer;
                buffer << std::put_time(&timeNow, format.c_str());

                return buffer.str();
            }

            static std::mutex guard;
        };

        std::mutex __Logger::guard;
    }
}

#endif // !LOGGER_HPP