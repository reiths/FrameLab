module;

export module Layers;

import framelab;

using namespace framelab::application;


export struct DockspaceLayer : public Layer
{

    DockspaceLayer();

    virtual auto on_update(Timestep) -> void override;

    virtual auto on_event(Event&) -> void override;

    virtual auto on_render() -> void override;
};


export struct AudioLayer : public Layer
{

    AudioLayer();

    virtual auto on_update(Timestep) -> void override;

    virtual auto on_event(Event&) -> void override;

    virtual auto on_render() -> void override;
};
