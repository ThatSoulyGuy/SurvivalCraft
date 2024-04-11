#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <fstream>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Core/Settings.hpp"
#include "Util/FileHelper.hpp"
#include "Util/Formatter.hpp"

using namespace SurvivalCraft::Core;
using namespace SurvivalCraft::Util;

namespace SurvivalCraft
{
	namespace Render
	{
		class Shader
		{

		public:
			
			void Generate()
			{
				program = glCreateProgram();

				GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexData);
				GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentData);

				glAttachShader(program, vertex);
				glAttachShader(program, fragment);
				glLinkProgram(program);
				CheckShaderLinking(program);

				glDeleteShader(vertex);
				glDeleteShader(fragment);
			}

			void Use()
			{
				glUseProgram(program);
			}

			void SetUniform(const std::string& name, bool value) const
			{
				glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
			}

			void SetUniform(const std::string& name, int value) const
			{
				glUniform1i(glGetUniformLocation(program, name.c_str()), value);
			}

			void SetUniform(const std::string& name, float value) const
			{
				glUniform1f(glGetUniformLocation(program, name.c_str()), value);
			}

			void SetUniform(const std::string& name, const glm::vec2& value) const
			{
				glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
			}

			void SetUniform(const std::string& name, const glm::vec3& value) const
			{
				glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
			}

			void SetUniform(const std::string& name, const glm::vec4& value) const
			{
				glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
			}

			void SetUniform(const std::string& name, const glm::mat2& value) const
			{
				glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
			}

			void SetUniform(const std::string& name, const glm::mat3& value) const
			{
				glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
			}

			void SetUniform(const std::string& name, const glm::mat4& value) const
			{
				glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
			}

			std::string GetName() const
			{
				return name;
			}

			void CleanUp()
			{
				glDeleteProgram(program);
			}

			static Shader Create(const std::string& name, const std::string& localPath, const std::string& domain = Settings::defaultDomain)
			{
				Shader out = {};

				out.name = name;
				out.vertexPath = Formatter::Format("Assets/{}/{}Vertex.glsl", domain, localPath);
				out.fragmentPath = Formatter::Format("Assets/{}/{}Fragment.glsl", domain, localPath);
				
				out.vertexData = FileHelper::ReadFile(out.vertexPath);
				out.fragmentData = FileHelper::ReadFile(out.fragmentPath);

				return out;
			}

		private:

			GLuint CompileShader(GLenum type, const std::string& source)
			{
				GLuint shader = glCreateShader(type);
				const char* sourceString = source.c_str();
				glShaderSource(shader, 1, &sourceString, NULL);
				glCompileShader(shader);

				GLint success;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

				if (!success)
				{
					GLchar infoLog[512];
					glGetShaderInfoLog(shader, 512, NULL, infoLog);
					Logger_ThrowError("nullptr", Formatter::Format("Shader compilation failed: {}", infoLog), true);
				}

				return shader;
			}

			static void CheckShaderLinking(GLuint program)
			{
				GLint success;
				glGetProgramiv(program, GL_LINK_STATUS, &success);

				if (!success)
				{
					GLchar infoLog[512];
					glGetProgramInfoLog(program, 512, NULL, infoLog);
					Logger_ThrowError("nullptr", Formatter::Format("Shader program linking failed: {}", infoLog), true);
				}
			}

			std::string name;

			std::string vertexPath, vertexData;
			std::string fragmentPath, fragmentData;

			GLuint program = 0;

		};
	}
}

#endif // !SHADER_HPP