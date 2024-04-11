#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "Core/Logger.hpp"
#include "Core/Settings.hpp"
#include "Util/Formatter.hpp"

using namespace SurvivalCraft::Core;
using namespace SurvivalCraft::Util;

namespace SurvivalCraft
{
	namespace Render
	{
		enum class TextureWrapMode
		{
			REPEAT = GL_REPEAT,
			MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
			CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
			CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
		};

		enum class TextureFilterMode
		{
			NEAREST = GL_NEAREST,
			LINEAR = GL_LINEAR
		};

		struct TextureData
		{
			TextureWrapMode wrapMode;
			TextureFilterMode filterMode;

			static TextureData Create(TextureWrapMode wrapMode, TextureFilterMode filterMode)
			{
				return { wrapMode, filterMode };
			}
		};

		class Texture
		{

		public:
			
			void Generate(TextureData data) 
			{
				glGenTextures(1, &id);
				glBindTexture(GL_TEXTURE_2D, id);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(data.wrapMode));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(data.wrapMode));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(data.filterMode));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(data.filterMode));

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

				stbi_set_flip_vertically_on_load(true);

				int width, height, nrChannels;
				unsigned char* imageData = stbi_load((newPath.string() + "/" + path).c_str(), &width, &height, &nrChannels, 0);

				if (imageData)
				{
					GLenum format = GL_RGB;

					if (nrChannels == 1)
						format = GL_RED;
					else if (nrChannels == 3)
						format = GL_RGB;
					else if (nrChannels == 4)
						format = GL_RGBA;

					glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
					glGenerateMipmap(GL_TEXTURE_2D);

					stbi_image_free(imageData);
				}
				else
					Logger_ThrowError("nullptr", Formatter::Format("Failed to load texture: {}", newPath.string() + "/" + path), true);
				
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			void Use()
			{
				glBindTexture(GL_TEXTURE_2D, id);
			}

			std::string GetName() const
			{
				return name;
			}

			void CleanUp()
			{
				glDeleteTextures(1, &id);
			}

			static Texture Create(const std::string& name, const std::string& localPath, const std::string& domain = Settings::defaultDomain)
			{
				Texture out = {};

				out.name = name;
				out.path = Formatter::Format("Assets/{}/{}", domain, localPath);

				return out;
			}

		private:
			
			std::string name;
			std::string path;

			GLuint id;

		};
	}
}

#endif // !TEXTURE_HPP