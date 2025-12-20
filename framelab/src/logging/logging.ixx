module;
#include <string_view>

export module framelab.logging;
export import :component_logger;


namespace framelab::logging {

	namespace components {

		/// @brief Component for the App logger.
		struct Application {
			static constexpr std::string_view name = "app";
		};
		
		/// @brief Component for the audio logger.
		struct Audio {
			static constexpr std::string_view name = "audio";
		};

		/// @brief Component for the core logger.
		struct Client {
			static constexpr std::string_view name = "client";
		};

	
	}

	/// @brief Application logger.
	export using app_log = ComponentLogger<components::Application, level::trace>;
	
	/// @brief Audio logger.
	export using audio_log = ComponentLogger<components::Audio, level::trace>;

	/// @brief Core logger.
	export using client_log = ComponentLogger<components::Client, level::trace>;

}