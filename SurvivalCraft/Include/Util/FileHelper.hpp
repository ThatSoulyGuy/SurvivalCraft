#ifndef FILE_HELPER_HPP
#define FILE_HELPER_HPP

#include <fstream>
#include <string>
#include <filesystem>
#include "Core/Logger.hpp"

using namespace SurvivalCraft::Core;

namespace SurvivalCraft
{
	namespace Util
	{
		class FileHelper
		{

		public:

            static std::string ReadFile(const std::string& path) 
            {
                std::filesystem::path newPath = std::filesystem::current_path();

#ifdef _DEBUG

                for (int d = 0; d < 3; ++d) 
                {
                    if (newPath.has_parent_path()) 
                        newPath = newPath.parent_path();
                    else 
                        break;
                }

#endif // DEBUG

                std::ifstream file(newPath.string() + "/" + path, std::ios::binary | std::ios::ate);
                
                Logger_WriteConsole("Reading file: " + newPath.string() + "/" + path, LogLevel::INFORMATION);

                if (!file) 
                    Logger_ThrowError("nullptr", "Could not open file: " + newPath.string() + "/" + path, false);
                
                std::streampos fileSize = file.tellg();
                std::string content(fileSize, '\0');
                file.seekg(0);

                if (file.read(content.data(), fileSize)) 
                    return content;
                else 
                    Logger_ThrowError("nullptr", "Could not read file: " + newPath.string() + "/" + path, false);
            }
		};
	}
}

#endif // !FILE_HELPER_HPP