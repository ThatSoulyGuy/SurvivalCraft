#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Core/Logger.hpp"

namespace SurvivalCraft
{
	namespace Core
	{
		class Window
		{

		public:
			
			void Create(const std::string& title, glm::ivec2 position, const glm::ivec2& size, glm::vec3 color)
			{
				this->color = color;

				if (!glfwInit())
					Logger_ThrowError("nullptr", "Failed to initialize GLFW", true);

				if (position.x == -1 && position.y == -1)
				{
					const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

					position.x = (mode->width - size.x) / 2;
					position.y = (mode->height - size.y) / 2;
				}
				
				glfwDefaultWindowHints();
				glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
				handle = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);

				if (!handle)
				{
					glfwTerminate();
					Logger_ThrowError("nullptr", "Failed to create window", true);
				}

				glfwMakeContextCurrent(handle);

				glfwSetFramebufferSizeCallback(handle, FramebufferSizeCallback);

				if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
				{
					glfwTerminate();
					Logger_ThrowError("nullptr", "Failed to initialize GLAD", true);
				}

				SetPosition(position);

				glEnable(GL_DEPTH_TEST);
			}
			
			void SetTitle(const std::string& title)
			{
				glfwSetWindowTitle(handle, title.c_str());
			}

			std::string GetTitle()
			{
				return std::string(glfwGetWindowTitle(handle));
			}

			void SetPosition(const glm::ivec2& position)
			{
				glfwSetWindowPos(handle, position.x, position.y);
			}

			glm::ivec2 GetPosition()
			{
				int x, y;

				glfwGetWindowPos(handle, &x, &y);

				return { x, y };
			}

			void SetSize(const glm::ivec2& size)
			{
				glfwSetWindowSize(handle, size.x, size.y);
			}

			glm::ivec2 GetSize()
			{
				int width, height;

				glfwGetWindowSize(handle, &width, &height);

				return { width, height };
			}

			void SetColor(const glm::vec3& color)
			{
				Window::color = color;
			}

			glm::vec3 GetColor()
			{
				return color;
			}

			float GetAspectRatio()
			{
				glm::ivec2 size = GetSize();

				if (size.y == 0) 
				{
					Logger_ThrowError("0", "Failed to calculate aspect ratio", false);
					return 0.0f;
				}

				return static_cast<float>(size.x) / static_cast<float>(size.y);
			}

			GLFWwindow* GetHandle()
			{
				return handle;
			}

			bool ShouldClose()
			{
				return glfwWindowShouldClose(handle);
			}

			void PreUpdate()
			{
				glClearColor(color.x, color.y, color.z, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			void PostUpdate()
			{
				glfwSwapBuffers(handle);
				glfwPollEvents();
			}

			void CleanUp()
			{
				glfwDestroyWindow(handle);
				glfwTerminate();
			}

		private:

			static void FramebufferSizeCallback(GLFWwindow* handle, int width, int height)
			{
				glViewport(0, 0, width, height);
			}

			glm::vec3 color = {};
			GLFWwindow* handle = nullptr;

		};
	}
}

#endif // !WINDOW_HPP