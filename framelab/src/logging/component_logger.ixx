/// @example
/// struct AppComponent {
///		static constexpr std::string_view name = "application";
/// };
///
/// using app_log = ComponentLogger<AppComponent, logging::level::info>;
///
/// app_log::info("Hello from component");
///
/// @todo maybe there could be a sink registry and init() can be used to add custom sinks

module;

#include <filesystem>
#include <memory>

#include <gsl/gsl>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>


export module framelab.logging:component_logger;


using spdlog::format_string_t;
using std::string_view;
using std::filesystem::path;
using logger_ptr = std::unique_ptr<spdlog::logger>;


namespace details
{
    // inline allows it to be defined in multiple translation units but ensures only one instance
    // exists at link time
    inline spdlog::sink_ptr console_sink;
    inline spdlog::sink_ptr file_sink;
} // namespace details


export namespace framelab::logging
{

    export using level = spdlog::level::level_enum;


    /// @brief Initialises the log sinks.
    ///
    /// @note Must be called before logging.
    void init(path log_file)
    {
        spdlog::set_level(level::trace);

        // Log sinks
        details::console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        details::console_sink->set_level(level::trace);

        // Rotating file sink: max 5 MB, keep 3 rotated files
        details::file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            log_file.string(), 1024 * 1024 * 5, 3);
        details::file_sink->set_level(level::trace);
    }


    /// @brief A Component is a Tag with just a name to distinguish component loggers.
    ///
    /// @example
    /// struct Main {
    ///		static constexpr std::string_view name = "application";
    /// };
    template <typename T>
    concept Component = std::is_empty_v<T> && requires(T) { // should be a pure tag type
        { T::name } -> std::convertible_to<string_view>;    // string_view name should exist
    };


    /// @brief Type-safe logger for a specific component with compile-time log level filtering.
    ///
    /// @tparam C Component type satisfying the `Component` concept (empty type with static
    /// constexpr name).
    /// @tparam LogLevel Minimum compile-time log level; messages below this are omitted.
    ///
    /// @warning Call `logging::init()` before using any logger.
    template <Component C, level LogLevel>
    struct ComponentLogger
    {
      private:
        static constexpr level level = LogLevel;
        logger_ptr logger;

      public:
        template <typename... Args>
        inline static constexpr void trace(format_string_t<Args...> fmt, Args&&... args) noexcept
        {
            log<level::trace>(fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static constexpr void debug(format_string_t<Args...> fmt, Args&&... args) noexcept
        {
            log<level::debug>(fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static constexpr void info(format_string_t<Args...> fmt, Args&&... args) noexcept
        {
            log<level::info>(fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static constexpr void warn(format_string_t<Args...> fmt, Args&&... args) noexcept
        {
            log<level::warn>(fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static constexpr void error(format_string_t<Args...> fmt, Args&&... args) noexcept
        {
            log<level::err>(fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static constexpr void critical(format_string_t<Args...> fmt, Args&&... args) noexcept
        {
            log<level::critical>(fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline static constexpr void assertion(bool result, format_string_t<Args...> fmt,
                                               Args&&... args)
        {
            if (!result) {
                log<level::critical>(fmt, std::forward<Args>(args)...);
                throw std::runtime_error{"Assertion failed, please check the logs."};
            }
        }

        /*
        template<typename... Args>
        inline static constexpr void assertion(bool result, const std::source_location& loc =
        std::source_location::current()) noexcept { if (!result) { log<level::err>("Precondition not
        fullfilled: {}, {}, line {}.", loc.function_name(), loc.file_name(), loc.line());
                }
        }
        */

      private:
        static ComponentLogger& get()
        {
            static ComponentLogger instance{create_logger()};
            return instance;
        }

        ComponentLogger(logger_ptr logger) : logger(std::move(logger)) {}

        ComponentLogger(const ComponentLogger&) = delete;
        ComponentLogger& operator=(const ComponentLogger&) = delete;

        static auto create_logger() -> logger_ptr
        {
            // init() should be called before any get()
            Expects(details::console_sink && details::file_sink);

            auto name = std::string(C::name);
            auto logger = std::make_unique<spdlog::logger>(
                name, std::initializer_list{details::console_sink, details::file_sink});
            logger->set_level(LogLevel);
            logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] [%n] %v");
            return logger;
        }

        template <logging::level MsgLevel, typename... Args>
        inline static constexpr auto log(format_string_t<Args...> fmt, Args&&... args) noexcept
            -> void
        {
            if constexpr (MsgLevel >= LogLevel) {
                get().logger->log(MsgLevel, fmt, std::forward<Args>(args)...);
            }
        }
    };

} // namespace framelab::logging