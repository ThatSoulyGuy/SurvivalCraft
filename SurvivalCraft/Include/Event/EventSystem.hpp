#ifndef EVENT_SYSTEM_HPP
#define EVENT_SYSTEM_HPP

#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <any>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define EventHeader(name) \
	class name : public Event \
	{ \
	public: \

#define EventFooter \
	}; \

namespace SurvivalCraft
{
	namespace Event
	{
		class Event {};

		EventHeader(PreInitializeEvent)

			explicit PreInitializeEvent() {}

		EventFooter;

		EventHeader(InitializeEvent)

			explicit InitializeEvent() {}
		
		EventFooter;

		EventHeader(CleanUpEvent)

			explicit CleanUpEvent() {}

		EventFooter;

		using EventListener = std::function<void(const Event&)>;

		class EventSystem
		{

		public:
			
			template<typename EventType>
			void Subscribe(const EventListener& listener) 
			{
				registeredListeners[typeid(EventType)].push_back(listener);
			}

			template<typename EventType>
			void Broadcast(const EventType& event) 
			{
				auto& eventTypeListeners = registeredListeners[typeid(EventType)];

				for (auto& listener : eventTypeListeners) 
					listener(event);
			}

		private:

			std::unordered_map<std::type_index, std::vector<EventListener>> registeredListeners;

		};
	}
}

#endif // !EVENT_SYSTEM_HPP