module;
#include <GLFW/glfw3.h>
#include <memory>
#include <ranges>

module framelab.application;

import framelab.logging;

using framelab::logging::app_log;


namespace framelab::application
{
    Application* Application::instance = nullptr;

    Application::Application()
        : window_(WindowProperties{.title = "Main", .width = 1200, .height = 900})
    {
        app_log::assertion(!instance, "Application already exists!");
        instance = this;

        window_.set_event_callback([this](Event& e) { on_event(e); });

        layer_stack_.push_overlay(std::make_unique<ImGuiLayer>(window_));

        app_log::trace("Application created.");
    }

    Application::~Application()
    {
        instance = nullptr;
        app_log::trace("Application terminates.");
    }

    inline Application& Application::get() noexcept
    {
        return *instance;
    }

    auto Application::run() -> void
    {
        while (running_) {
            Window::poll_all_events();

            double time = glfwGetTime(); // @todo can we get glfw out of this class?
            Timestep timestep{time - last_frame_time_};
            last_frame_time_ = time;

            if (!window_.minimized()) {
                for (auto const& layer : layer_stack_) {
                    layer->on_update(timestep);
                }

                ImGuiLayer::begin_frame(window_);
                for (auto const& layer : layer_stack_) {
                    layer->on_render();
                }
                ImGuiLayer::end_frame(window_);
            }
        }
    }

    auto Application::push_layer(std::unique_ptr<Layer> layer) noexcept -> void
    {
        layer_stack_.push_layer(move(layer));
    }

    auto Application::on_event(Event& e) -> void
    {
        EventDispatcher dispatcher{e};
        dispatcher.dispatch<WindowCloseEvent>(
            [this](WindowCloseEvent& e) { return this->on_close(e); });

        if (e.handled) {
            return;
        }

        // propagate event through the layerstack, top to bottom
        for (auto const& layer : layer_stack_ | std::views::reverse) {
            layer->on_event(e);
            if (e.handled) {
                return;
            }
        }

        app_log::warn("Unhandled event: {}", typeid(e).name());
    }

    auto Application::on_close(WindowCloseEvent&) noexcept -> bool
    {
        running_ = false;
        return true;
    }

} // namespace framelab::application
