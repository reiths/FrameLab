module;
#include <imgui.h>

module Layers;


DockspaceLayer::DockspaceLayer()
	: Layer("Dockspace Layer")
{
}

auto DockspaceLayer::on_update(Timestep) -> void {
}

auto DockspaceLayer::on_event(Event&) -> void {
    //EventDispatcher dispatcher{ e };
}

auto DockspaceLayer::on_render() -> void {
    constexpr const ImGuiWindowFlags flags =
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus;

    const ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("Dockspace Host", nullptr, flags);

    ImGui::PopStyleVar(2);

    ImGuiID dockspaceID = ImGui::GetID("MainDockspace");
    ImGui::DockSpace(dockspaceID, ImVec2(0, 0), ImGuiDockNodeFlags_None /* | ImGuiDockNodeFlags_PassthruCentralNode */);

    ImGui::End();
}


AudioLayer::AudioLayer()
    : Layer("Audio Layer")
{
}

auto AudioLayer::on_update(Timestep) -> void {
}

auto AudioLayer::on_event(Event&) -> void {
    //EventDispatcher dispatcher{ e };
}

auto AudioLayer::on_render() -> void {
    ImGui::Begin("Audio");
    ImGui::End();
}
