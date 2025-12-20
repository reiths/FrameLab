module;
#include <memory>
#include <span>
#include <string>

export module framelab.application;
export import :window;
export import :layer;
export import :timestep;
export import :imgui;
export import :events;

import framelab.logging;

using std::string;
using framelab::logging::app_log;


export namespace framelab::application {

	/// @brief Simple wrapper for better handling of command line arguments.
	using CommandLineArgs = std::span<char* const>;


	class Application {
		bool running_{ true };
		double last_frame_time_{ 0.0f };
		Window window_;
		LayerStack layer_stack_;
		static Application* instance;

	public:
		explicit Application();
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		virtual ~Application();

		[[nodiscard]]
		inline static Application& get() noexcept;

		auto run() -> void;
		auto push_layer(std::unique_ptr<Layer> layer) noexcept -> void;
		auto on_event(Event& e) -> void;

	private:
		auto on_close(WindowCloseEvent&) noexcept -> bool;
	};

}