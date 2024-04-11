#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Core/Window.hpp"
#include "Event/EventSystem.hpp"
#include "Math/Transform.hpp"

using namespace SurvivalCraft::Core;
using namespace SurvivalCraft::Event;
using namespace SurvivalCraft::Math;

namespace SurvivalCraft
{
	namespace Render
	{
		class Camera;

		EventHeader(CameraCreationEvent)

			Camera& camera;

			explicit CameraCreationEvent(Camera& camera) : camera(camera) {}

		EventFooter;

		class Camera
		{

		public:
			
			glm::mat4 GetProjectionMatrix() const
			{
				return glm::perspective(glm::radians(fieldOfView), window.GetAspectRatio(), nearPlane, farPlane);
			}

			glm::mat4 GetViewMatrix() const
			{
				return glm::lookAt(transform.position, transform.position + transform.GetForward(), transform.GetWorldUp());
			}

			float GetFieldOfView() const 
			{ 
				return fieldOfView; 
			}

			float GetNearPlane() const 
			{ 
				return nearPlane; 
			}

			float GetFarPlane() const 
			{ 
				return farPlane; 
			}

			static void StaticInitialize(Window& window, EventSystem& eventSystem)
			{
				Camera::window = window;
				Camera::eventSystem = eventSystem;
			}

			static Camera Create(const glm::vec3& position, const glm::vec3& rotation, float fieldOfView, float nearPlane, float farPlane)
			{
				Camera out = {};

				out.transform = Transform::Create(position, rotation, { 1.0f, 1.0f, 1.0f });

				eventSystem.Broadcast(CameraCreationEvent(out));

				return out;
			}

			Transform transform;

		private:

			float fieldOfView; 
			float nearPlane; 
			float farPlane;

			static Window window;
			static EventSystem eventSystem;

		};

		Window Camera::window;
		EventSystem Camera::eventSystem;
	}
}

#endif // !CAMERA_HPP