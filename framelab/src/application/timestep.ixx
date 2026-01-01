export module framelab.application:timestep;


export namespace framelab::application
{
    // TODO maybe use a proper units library? std::chrono?
    class Timestep
    {
        double time_;

      public:
        explicit Timestep(double time) noexcept : time_(time) {}
        operator double() const noexcept { return time_; }
        auto seconds() const noexcept -> double { return time_; }
        auto milliseconds() const noexcept -> double { return time_ * 1000.0f; }
    };

} // namespace framelab::application