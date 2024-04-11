#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace SurvivalCraft
{
	namespace Math
	{
		class Transform
		{

		public:

			glm::vec3 GetForward() const 
			{
				glm::vec4 forward = GetMatrix() * glm::vec4{ 0.0f, 0.0f, 1.0f, 0.0f };

				return glm::vec3(forward);
			}

			glm::vec3 GetRight() const 
			{
				glm::vec4 right = GetMatrix() * glm::vec4{ 1.0f, 0.0f, 0.0f, 0.0f };

				return glm::vec3(right);
			}

			glm::vec3 GetUp() const 
			{
				glm::vec4 up = GetMatrix() * glm::vec4{ 0.0f, 1.0f, 0.0f, 0.0f };

				return glm::vec3(up);
			}

			glm::vec3 GetWorldUp() const 
			{
				return glm::vec3(0.0f, 1.0f, 0.0f);
			}

			glm::mat4 GetMatrix() const 
			{
				glm::mat4 out = glm::mat4(1.0f);

				out = glm::scale(out, scale);

				out = glm::rotate(out, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				out = glm::rotate(out, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				out = glm::rotate(out, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				
				out = glm::translate(out, position);

				return out;
			}
			
            static Transform Create(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) 
			{
				Transform out = {};

				out.position = position;
				out.rotation = rotation;
				out.scale = scale;

				return out;
			}

			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;

		private:
			


		};
	}
}

#endif // !TRANSFORM_HPP