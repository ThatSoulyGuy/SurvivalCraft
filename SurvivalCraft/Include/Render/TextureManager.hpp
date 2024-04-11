#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <unordered_map>
#include "Event/EventSystem.hpp"
#include "Render/Texture.hpp"

using namespace SurvivalCraft::Event;

namespace SurvivalCraft
{
	namespace Render
	{
		EventHeader(TextureRegistrationEvent)

			Texture& texture;

			explicit TextureRegistrationEvent(Texture& texture) : texture(texture) {}

		EventFooter;

		EventHeader(TextureGenerationEvent)

			Texture& texture;

			explicit TextureGenerationEvent(Texture& texture) : texture(texture) {}

		EventFooter;

		EventHeader(TextureDeletionEvent)

			const std::string& name;

			explicit TextureDeletionEvent(const std::string& name) : name(name) {}

		EventFooter;

		class TextureManager
		{

		public:
			
			static void StaticInitialize(EventSystem& eventSystem)
			{
				TextureManager::eventSystem = eventSystem;
			}

			void Register(const Texture& texture)
			{
				registeredTextures[texture.GetName()] = texture;

				eventSystem.Broadcast(TextureRegistrationEvent(registeredTextures[texture.GetName()]));
			}

			Texture Get(const std::string& name)
			{
				return registeredTextures[name];
			}

		private:
			
			std::unordered_map<std::string, Texture> registeredTextures;

			static EventSystem eventSystem;

		};

		EventSystem TextureManager::eventSystem;
	}
}

#endif // !TEXTURE_MANAGER_HPP