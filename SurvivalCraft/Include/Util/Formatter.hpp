#ifndef FORMAT_HPP
#define FORMAT_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ranges>
#include <regex>
#include <unordered_map>

namespace SurvivalCraft
{
	namespace Util
	{
		class Formatter
		{

		public:
			
            template<typename... Args>
			static std::string Format(const std::string& format, Args... args)
			{
                std::ostringstream stream;
                std::vector<std::string> arguments = { args... };
                size_t argumentIndex = 0;

                for (size_t f = 0; f < format.length(); ++f) 
                {
                    if (format[f] == '{' && f + 1 < format.length() && format[f + 1] == '}') 
                    {
                        if (argumentIndex < arguments.size()) 
                        {
                            stream << arguments[argumentIndex++];
                            f++;
                        }
                        else 
                            throw std::runtime_error("Not enough arguments provided for format string");
                    }
                    else 
                        stream << format[f];
                }

                return stream.str();
			}

            static std::string ColorFormat(const std::string& format)
            {
                const std::unordered_map<char, std::string> codes = 
                {
                    {'0', "\x1b[30m"}, 
                    {'1', "\x1b[34m"}, 
                    {'2', "\x1b[32m"},
                    {'3', "\x1b[36m"}, 
                    {'4', "\x1b[31m"}, 
                    {'5', "\x1b[35m"},
                    {'6', "\x1b[33m"}, 
                    {'7', "\x1b[37m"}, 
                    {'8', "\x1b[90m"},
                    {'9', "\x1b[94m"}, 
                    {'a', "\x1b[92m"}, 
                    {'b', "\x1b[96m"},
                    {'c', "\x1b[91m"}, 
                    {'d', "\x1b[95m"}, 
                    {'e', "\x1b[93m"},
                    {'f', "\x1b[97m"}, 
                    {'l', "\x1b[1m"}, 
                    {'n', "\x1b[4m"},
                    {'o', "\x1b[3m"}, 
                    {'r', "\x1b[0m"}
                };

                std::string result;
                bool escape = false;

                for (char character : format) 
                {
                    if (character == '&') 
                        escape = true;
                    else if (escape) 
                    {
                        if (auto codeIterator = codes.find(character); codeIterator != codes.end()) 
                            result += codeIterator->second;
                        
                        escape = false;
                    }
                    else 
                        result += character;
                }

                return result;
            }

            static std::string ColorDeformat(const std::string& format)
            {
                std::string result = std::regex_replace(format, std::regex("§."), "");

                result = std::regex_replace(result, std::regex("\x1b\\[[0-9;]*[mGKH]"), "");

                return result;
            }
		};
	}
}

#endif // !FORMAT_HPP