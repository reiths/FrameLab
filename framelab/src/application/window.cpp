module;
#include <atomic>
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>

module framelab.application;

import framelab.logging;

using framelab::logging::app_log;


namespace framelab::application {

	void GlfwContext::glfw_error_callback(int error, const char* description) {
		app_log::critical("GLFW Error {}: {}.", error, description);
	}

	GlfwContext::GlfwContext() {
		// increment ref_count and check previous value
		if (ref_count_.fetch_add(1, std::memory_order_acq_rel) == 0) {
			glfwSetErrorCallback(glfw_error_callback);
			app_log::assertion(glfwInit(), "GLFW failed to initialise.");
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
			app_log::trace("GLFW Context initialised.");
		}
	}

	GlfwContext::~GlfwContext() {
		// decrement and if this was the last, terminate
		if (ref_count_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
			app_log::trace("GLFW Context terminates.");
			glfwTerminate();
		}
	}

	Window::Window(WindowProperties properties, GLFWwindow* shared)
		: glfw_(), properties_(std::move(properties))
	{
		main_scale_ = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
		native_window_ = glfwCreateWindow(
			static_cast<int>(properties_.width * main_scale_),
			static_cast<int>(properties_.height * main_scale_),
			properties_.title.c_str(),
			nullptr,
			shared
		);
		glfwMakeContextCurrent(native_window_);
		set_vsync(properties_.vsync_enabled);

		// we need to work with this cause the callbacks don't work with lambdas capturing this
		glfwSetWindowUserPointer(native_window_, this);

		glfwSetWindowCloseCallback(native_window_, [](GLFWwindow* window) {
			auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent e;
			if (self->event_callback_) {
				self->event_callback_(e);
			}
		});

		glfwSetWindowSizeCallback(native_window_, [](GLFWwindow* window, int width, int height) {
			auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
			//WindowResizeEvent event{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
			//self->event_callback_(event);
			self->properties_.width = width;
			self->properties_.height = height;
		});

		app_log::trace("Window {} initialised.", properties_.title);
	}

	Window::~Window() {
		app_log::trace("Window '{}' terminates.", properties_.title);
		glfwDestroyWindow(native_window_);
	}

	// The reason we set the event callback instead of calling Application::get().on_event directly is circular imports and better extendability.
	void Window::set_event_callback(EventCallback const& cb) noexcept {
		event_callback_ = move(cb);
	}

	auto Window::poll_all_events() -> void {
		glfwPollEvents();
	}

	auto Window::on_update() -> void {
	}

	auto Window::set_vsync(bool enabled) noexcept -> void {
		enabled ? glfwSwapInterval(1) : glfwSwapInterval(0);
		properties_.vsync_enabled = enabled;
	}

}