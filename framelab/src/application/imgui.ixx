module;
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <gsl/gsl>
#include <GLFW/glfw3.h>

export module framelab.application:imgui;

import framelab.logging;
import :layer;
import :window;

using gsl::not_null;
using framelab::logging::app_log;


export namespace framelab::application {
	
	class ImguiContext {
		// used so imgui is initialised only once, ever with multiple windows
		inline static std::atomic_int ref_count_{ 0 };

	public:
		ImguiContext(Window const& window) {
			// increment ref_count and check previous value
			if (ref_count_.fetch_add(1, std::memory_order_acq_rel) == 0) {
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO();
				io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
				io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
				io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
				//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
				ImGui::StyleColorsDark();
				ImGuiStyle& style = ImGui::GetStyle();
				style.ScaleAllSizes(window.main_scale());
				style.FontScaleDpi = window.main_scale();
				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
					style.WindowRounding = 0.0f;
					style.Colors[ImGuiCol_WindowBg].w = 1.0f;
				}
				ImGui_ImplGlfw_InitForOpenGL(window.native_window(), true);
				ImGui_ImplOpenGL3_Init(glsl_version.data());
				app_log::trace("Imgui Context initialised.");
			}
		}

		~ImguiContext() {
			// decrement and if this was the last, terminate
			if (ref_count_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
				app_log::trace("Imgui Context terminates.");
				ImGui_ImplOpenGL3_Shutdown();
				ImGui_ImplGlfw_Shutdown();
				ImGui::DestroyContext();
			}
		}
	};


	class ImGuiLayer : public Layer {
		ImguiContext imgui_;
		bool block_events_{ true };
		float time_{ 0.0f };

	public:
		ImGuiLayer(Window const& window);
		~ImGuiLayer();

		//virtual auto on_event() -> void override;

		auto block_events(bool block) -> void { block_events_ = block; }
		
		static void begin_frame(Window const& window) noexcept;
		static void end_frame(Window const& window) noexcept;
	};



}


namespace framelab::application {
	ImGuiLayer::ImGuiLayer(Window const& window)
		: Layer("ImGuiLayer"), imgui_(window)
	{
		app_log::trace("ImGuiLayer initialised.");
	}

	ImGuiLayer::~ImGuiLayer() {
		app_log::trace("ImGuiLayer terminates.");
	}

	void ImGuiLayer::begin_frame(Window const& window) noexcept {
		glfwMakeContextCurrent(window.native_window());
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::end_frame(Window const& window) noexcept {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2{ static_cast<float>(window.width()), static_cast<float>(window.height()) };

		ImGui::Render();
		constexpr ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		int display_w, display_h;
		glfwGetFramebufferSize(window.native_window(), &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(window.native_window());
	}

}