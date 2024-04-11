#ifndef ENTITY_PLAYER_HPP
#define ENTITY_PLAYER_HPP

#include "Core/InputManager.hpp"
#include "Render/Camera.hpp"

using namespace SurvivalCraft::Core;
using namespace SurvivalCraft::Render;

namespace SurvivalCraft
{
	namespace Entity 
	{
		class EntityPlayer
		{

		public:
			
			void Update()
			{
				UpdateMouseLook();
				UpdateMovement();

				camera.transform.position = transform.position;
				camera.transform.rotation = transform.rotation;

				Logger_WriteConsole(Formatter::Format("Player rotation: [{}, {}, {}]", std::to_string(transform.rotation.x), std::to_string(transform.rotation.y), std::to_string(transform.rotation.z)), LogLevel::DEBUGGING);
			}

			static void StaticInitialize(InputManager& inputManager)
			{
				EntityPlayer::inputManager = inputManager;
			}

			static EntityPlayer Create(const std::string& customName, const glm::vec3& position, const glm::vec3& rotation)
			{
				EntityPlayer out = {};

				inputManager.SetMouseState(false);

				out.customName = customName;
				out.camera = Camera::Create(position, rotation, 45.0f, 0.01f, 200.0f);
				out.transform = Transform::Create(position, rotation, { 1.0f, 1.0f, 1.0f });

				return out;
			}

			float speed = 0.1f;

			std::string customName = "player";

			Camera camera;
			Transform transform;

		private:

			void UpdateMouseLook()
			{
				glm::vec2 mouseDelta = inputManager.GetMouseDelta();

				float sensitivity = 0.1f;

				transform.rotation.x -= mouseDelta.y * sensitivity;
				transform.rotation.y += mouseDelta.x * sensitivity;

				const float maxPitch = 89.0f;
				transform.rotation.x = std::clamp(transform.rotation.x, -maxPitch, maxPitch);

				transform.rotation.y = fmod(transform.rotation.y, 360.0f);
			}

			void UpdateMovement()
			{
				glm::vec3 direction = { 0.0f, 0.0f, 0.0f };

				if (inputManager.GetKey(KeyCode::W, KeyState::PRESSED))
					direction += transform.GetForward();

				if (inputManager.GetKey(KeyCode::S, KeyState::PRESSED))
					direction -= transform.GetForward();

				if (inputManager.GetKey(KeyCode::A, KeyState::PRESSED))
					direction -= transform.GetRight();

				if (inputManager.GetKey(KeyCode::D, KeyState::PRESSED))
					direction += transform.GetRight();

				if (glm::length(direction) > 0.0f)
				{
					direction = glm::normalize(direction);
					transform.position += direction * speed;
				}
			}

			static InputManager inputManager;

		};

		InputManager EntityPlayer::inputManager;
	}
}

#endif // !ENTITY_PLAYER_HPP