module;
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

export module framelab.application:layer;

import :events;
import :timestep;

using std::vector, std::optional, std::move;


export namespace framelab::application
{
    class Layer
    {
      protected:
        std::string name_; // only for debug purposes

      public:
        explicit Layer(const std::string& name) : name_(name) {}
        virtual ~Layer() {}

        virtual auto on_update(Timestep) -> void {}
        virtual auto on_render() -> void {}
        virtual auto on_event(Event&) -> void {}

        inline auto name() const { return name_; }
    };


    class LayerStack
    {
        using layer_ptr = std::unique_ptr<Layer>;

        vector<layer_ptr> layers_;
        size_t insert_index_{0};

      private:
        auto view() & { return std::views::all(layers_); }
        auto view() const& { return std::views::all(layers_); }

      public:
        LayerStack() = default;
        ~LayerStack() = default;

        auto push_layer(layer_ptr&& layer) noexcept -> void;
        auto push_overlay(layer_ptr&& overlay) noexcept -> void;
        auto pop_layer(layer_ptr const& layer) noexcept -> void;
        auto pop_overlay(layer_ptr const& overlay) noexcept -> void;

        auto begin() { return std::ranges::begin(view()); }
        auto end() { return std::ranges::end(view()); }

        auto begin() const { return std::ranges::begin(view()); }
        auto end() const { return std::ranges::end(view()); }
    };

} // namespace framelab::application


// This enables LayerStack to be iterated over as std::view,
// e.g. for (auto const& layer : layer_stack_ | std::views::reverse);
export template <>
inline constexpr bool std::ranges::enable_borrowed_range<framelab::application::LayerStack> = true;


namespace framelab::application
{

    auto LayerStack::push_layer(layer_ptr&& layer) noexcept -> void
    {
        layers_.emplace(layers_.begin() + insert_index_, move(layer));
    }

    auto LayerStack::push_overlay(layer_ptr&& overlay) noexcept -> void
    {
        layers_.emplace_back(move(overlay));
    }

    auto LayerStack::pop_layer(layer_ptr const& layer) noexcept -> void
    {
        auto first = layers_.begin();
        auto last = layers_.begin() + insert_index_;
        if (auto it = std::ranges::find(first, last, layer); it != last) {
            layers_.erase(it);
            --insert_index_;
        }
    }

    auto LayerStack::pop_overlay(layer_ptr const& overlay) noexcept -> void
    {
        auto first = layers_.begin() + insert_index_;
        auto last = layers_.end();
        if (auto it = std::ranges::find(first, last, overlay); it != last) {
            layers_.erase(it);
        }
    }

} // namespace framelab::application