module;
#include <atomic>
#include <imgui.h>

export module framelab.application:imgui;

import framelab.logging;
import :layer;
import :window;

using framelab::logging::app_log;


export namespace framelab::application
{
    class ImguiContext
    {
        // used so imgui is initialised only once, even with multiple windows
        inline static std::atomic_int ref_count_{0};

      public:
        ImguiContext(Window const& window);
        ~ImguiContext();
    };


    class ImGuiLayer : public Layer
    {
        ImguiContext imgui_;
        bool block_events_{true};
        float time_{0.0f};

      public:
        ImGuiLayer(Window const& window);
        ~ImGuiLayer();

        // virtual auto on_event() -> void override;

        auto block_events(bool block) -> void { block_events_ = block; }

        static void begin_frame(Window const& window) noexcept;
        static void end_frame(Window const& window) noexcept;
    };

} // namespace framelab::application