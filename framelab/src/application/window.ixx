module;
#include <string>
#include <string_view>
#include <gsl/gsl>
#include <GLFW/glfw3.h>

export module framelab.application:window;

import framelab.logging;
import :events;

using std::string;
using gsl::not_null;


export namespace framelab::application {

	constexpr std::string_view glsl_version = "#version 410";
	

	struct WindowProperties {
		string title{ "" };
		uint32_t width{ 2560u };
		uint32_t height{ 1600u };
		bool fullscreen{ false };
		bool vsync_enabled{ true };
	};


	/// @note logging needs to be initialized first
	class GlfwContext {
		// used so glfw is initialised only once, ever with multiple windows
		inline static std::atomic_int ref_count_{ 0 };

		static void glfw_error_callback(int error, const char* description); 

	public:
		GlfwContext();
		~GlfwContext();
	};


	class Window {
		using EventCallback = std::function<void(Event&)>;

		GlfwContext glfw_;
		WindowProperties properties_;
		GLFWwindow* native_window_;
		float main_scale_;
		bool minimized_{ false };
		EventCallback event_callback_;

	public:
		Window(WindowProperties properties, GLFWwindow* shared = nullptr);
		~Window();

		void set_event_callback(EventCallback const& cb) noexcept;

		static auto poll_all_events() -> void;
		auto on_update() -> void;

		inline auto width() const -> uint32_t { return properties_.width; }
		inline auto height() const -> uint32_t { return properties_.height; }
		inline auto minimized() const -> bool { return minimized_; }
		inline auto main_scale() const -> float { return main_scale_; }
		inline auto native_window() const noexcept -> not_null<GLFWwindow*> { return native_window_; }

		auto set_vsync(bool enabled) noexcept -> void;
		inline auto vsync() const noexcept -> bool { return properties_.vsync_enabled; }
	};

}