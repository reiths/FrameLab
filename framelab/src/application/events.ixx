module;
#include <concepts>
#include <spdlog/fmt/fmt.h>

export module framelab.application:events;


export namespace framelab::application {

	/// @brief Base event type for all events.
	struct Event {
		bool handled{ false };
		constexpr virtual ~Event() noexcept = default;
	};


	/// @brief Concept for event types.
	template <typename T>
	concept EventType = std::derived_from<T, Event>;

	/// @brief Concept for event handler functions: bool func(Event&)
	template<typename Func, typename E>
	concept EventHandlerType =
		EventType<E> &&
		std::invocable<Func, E&>
		&& std::convertible_to<std::invoke_result_t<Func, E&>, bool>;


	/// @brief Type-safe event dispatcher.
	class EventDispatcher final {
		Event& event_;

	public:
		constexpr explicit EventDispatcher(Event& e) noexcept
			: event_(e)
		{
		}

		template<EventType E, typename Func> requires EventHandlerType<Func, E>
		constexpr bool dispatch(Func&& handler) noexcept(noexcept(handler(std::declval<E&>()))) {
			if (auto event = dynamic_cast<E*>(&event_)) {
				event->handled |= handler(*event);
				return true; // dispatched, i.e. event was of type E
			}
			return false;
		}
	};


	struct WindowCloseEvent : public Event {
	};

	struct WindowResizeEvent : public Event {
		uint32_t width;
		uint32_t height;
		
		constexpr WindowResizeEvent(uint32_t width, uint32_t height);
	};
	
}


// Template specializations so spdlog can handle Events
namespace fmt {

	template<>
	struct formatter<framelab::application::Event> : formatter<std::string> {
		auto format(framelab::application::Event, format_context& ctx) const -> decltype(ctx.out()) {
			return format_to(ctx.out(), "Event base class.");
		}
	};

	template<>
	struct formatter<framelab::application::WindowCloseEvent> : formatter<std::string> {
		auto format(framelab::application::WindowCloseEvent, format_context& ctx) const -> decltype(ctx.out()) {
			return format_to(ctx.out(), "WindowCloseEvent.");
		}
	};

}