module;

#include <expected>

export module result;


export {
    template <typename T, typename E>
    using Result = std::expected<T, E>;


    template <typename E>
    using Err = std::unexpected<E>;
}