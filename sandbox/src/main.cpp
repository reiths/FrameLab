#include <imgui.h>
#include <memory>

import framelab;
import Layers;

using namespace framelab::application;


class SandboxApp : public Application
{
  public:
    SandboxApp()
    {
        set_dark_theme();
        push_layer(std::make_unique<DockspaceLayer>());
        push_layer(std::make_unique<AudioLayer>());
    }

    auto set_dark_theme() -> void
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        // Base Colors
        colors[ImGuiCol_Text] = ImVec4(0.96f, 0.96f, 0.97f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.55f, 0.60f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.13f, 0.15f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.10f, 0.12f, 1.00f);

        // Neutral Surfaces
        colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.19f, 0.22f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.26f, 0.30f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.32f, 0.33f, 0.40f, 1.00f);

        // Orange Accents
        ImVec4 accent1 = ImVec4(1.00f, 0.40f, 0.10f, 1.00f); // deeper orange
        ImVec4 accent2 = ImVec4(1.00f, 0.65f, 0.30f, 1.00f); // warm orange
        ImVec4 accent3 = ImVec4(1.00f, 0.55f, 0.20f, 1.00f); // bright orange

        // Buttons
        colors[ImGuiCol_Button] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.32f, 0.38f, 1.00f);
        colors[ImGuiCol_ButtonActive] = accent1;

        // Sliders
        colors[ImGuiCol_SliderGrab] = accent2;
        colors[ImGuiCol_SliderGrabActive] = accent3;

        // Checkmarks
        colors[ImGuiCol_CheckMark] = accent1;

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4(0.17f, 0.19f, 0.22f, 1.00f);
        colors[ImGuiCol_TabHovered] = accent2;
        colors[ImGuiCol_TabActive] = ImVec4(0.22f, 0.24f, 0.28f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.14f, 0.15f, 0.17f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.20f, 0.23f, 1.00f);

        // Headers (Tree nodes, sections)
        colors[ImGuiCol_Header] = ImVec4(0.22f, 0.24f, 0.28f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = accent2;
        colors[ImGuiCol_HeaderActive] = accent1;

        // Separators
        colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.32f, 0.36f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = accent1;
        colors[ImGuiCol_SeparatorActive] = accent3;

        // Resize grips
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.22f, 0.24f, 0.28f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered] = accent1;
        colors[ImGuiCol_ResizeGripActive] = accent3;

        // Scrollbars
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.32f, 0.36f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = accent2;
        colors[ImGuiCol_ScrollbarGrabActive] = accent3;

        // Title bars
        colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.09f, 0.10f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = accent1;
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.07f, 0.07f, 0.08f, 1.00f);

        // Styling
        style.WindowRounding = 7.0f;
        style.FrameRounding = 6.0f;
        style.PopupRounding = 6.0f;
        style.GrabRounding = 5.0f;
        style.TabRounding = 6.0f;
        style.ScrollbarRounding = 9.0f;

        style.FrameBorderSize = 0.0f;
        style.WindowBorderSize = 1.0f;
        style.PopupBorderSize = 1.0f;

        style.WindowPadding = ImVec2(10, 10);
        style.FramePadding = ImVec2(8, 4);
        style.ItemSpacing = ImVec2(8, 6);
    }
};


int main(size_t argc, char** argv)
{
    CommandLineArgs args{argv, argc};

    framelab::logging::init("app.log");

    // client_log::info("Hello Sandbox");

    SandboxApp app{};

    app.run();
}