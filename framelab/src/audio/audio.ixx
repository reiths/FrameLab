module;
#include <whisper.h>
#include <memory>
#include <filesystem>

export module framelab.audio;

import framelab.logging;

using framelab::logging::audio_log;


namespace framelab::audio {

	/// @brief Whisper Speech-To-Text Model
	class Whisper {
		whisper_context* context_;

	private:
		explicit Whisper(std::filesystem::path const& model_file) {
			context_ = whisper_init_from_file_with_params(model_file.string().c_str(), whisper_context_default_params());
			audio_log::assertion(context_, "Could not load whisper model {}", model_file.string());
		}

		~Whisper() {
			whisper_free(context_);
		}
	};

}