#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP

#include <string>
#include <unordered_map>
#include "Event/EventSystem.hpp"
#include "Render/Shader.hpp"

using namespace SurvivalCraft::Event;

namespace SurvivalCraft
{
	namespace Render
	{
		class ShaderManager;

		EventHeader(ShaderRegistrationEvent)

			Shader& shader;

			explicit ShaderRegistrationEvent(Shader& shader) : shader(shader) {}

		EventFooter;

		EventHeader(ShaderGenerationEvent)

			Shader& shader;

			explicit ShaderGenerationEvent(Shader& shader) : shader(shader) {}

		EventFooter;

		EventHeader(ShaderDeletionEvent)

			const std::string& name;

			explicit ShaderDeletionEvent(const std::string& name) : name(name) {}

		EventFooter;

		class ShaderManager
		{

		public:
			
			static void StaticInitialize(EventSystem& eventSystem)
			{
				ShaderManager::eventSystem = eventSystem;
			}

			void Register(const Shader& shader)
			{
				registeredShaders[shader.GetName()] = shader;

				registeredShaders[shader.GetName()].Generate();

				eventSystem.Broadcast(ShaderRegistrationEvent(registeredShaders[shader.GetName()]));
				eventSystem.Broadcast(ShaderGenerationEvent(registeredShaders[shader.GetName()]));
			}

			Shader& Get(const std::string& name)
			{
				return registeredShaders[name];
			}

			void CleanUp()
			{
				for (auto& [key, shader] : registeredShaders)
				{
					shader.CleanUp();

					eventSystem.Broadcast(ShaderDeletionEvent(key));
				}
			}

		private:
			
			std::unordered_map<std::string, Shader> registeredShaders;

			static EventSystem eventSystem;

		};

		EventSystem ShaderManager::eventSystem;
	}
}

#endif // SHADER_MANAGER_HPP