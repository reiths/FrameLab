module;
#include <array>
#include <atomic>
#include <gsl/gsl>

export module framelab.channel;

using gsl::not_null;
using std::array, std::atomic;


namespace framelab::channel
{
    export template <typename T, size_t Capacity>
    class SpscRingBuffer
    {
        static_assert(Capacity > 1, "Capacity must be > 1");
        array<T, Capacity> buffer_;
        atomic<size_t> head_;
        atomic<size_t> tail_;

        static constexpr auto increment(size_t idx) noexcept -> size_t
        {
            return (idx + 1) % Capacity;
        }

      public:
        constexpr SpscRingBuffer() noexcept : head_(0), tail_(0) {}

        /// Returns false if the buffer is full
        auto push(T const& value) noexcept -> bool
        {
            const size_t head = head_.load(std::memory_order_relaxed);
            const size_t next = increment(head);
            const size_t tail = tail_.load(std::memory_order_acquire);

            if (next == tail) [[unlikely]] {
                return false; // full
            }

            buffer_[head] = value;
            head_.store(next, std::memory_order_release);
            return true;
        }

        /// Returns false if the buffer is full
        auto push(T&& value) noexcept -> bool
        {
            const size_t head = head_.load(std::memory_order_relaxed);
            const size_t next = increment(head);
            const size_t tail = tail_.load(std::memory_order_acquire);

            if (next == tail) [[unlikely]] {
                return false; // full
            }

            buffer_[head] = std::move(value);
            head_.store(next, std::memory_order_release);
            return true;
        }

        auto pop(T& out) noexcept -> bool
        {
            const size_t tail = tail_.load(std::memory_order_relaxed);
            const size_t head = head_.load(std::memory_order_acquire);

            if (tail == head) [[unlikely]] {
                return false; // empty
            }

            out = std::move(buffer_[tail]);
            tail_.store(increment(tail), std::memory_order_release);
            return true;
        }

        auto empty() const noexcept -> bool
        {
            return head_.load(std::memory_order_acquire) == tail_.load(std::memory_order_acquire);
        }

        auto full() const noexcept -> bool
        {
            // TODO doublecheck if this is correct
            return increment(head_.load(std::memory_order_acquire)) ==
                   tail_.load(std::memory_order_acquire);
        }

        constexpr auto capacity() const noexcept -> size_t { return Capacity; }
    };

    export template <typename T, size_t N>
    struct Frame
    {
        array<T, N> data;
        size_t size{0};
        atomic<int> refcount{0};
        atomic<bool> published{false};

        void reset_for_write() noexcept
        {
            size = 0;
            published.store(false, std::memory_order_relaxed);
            refcount.store(0, std::memory_order_relaxed);
        }
    };

    export template <typename T, size_t N>
    class FrameHandle
    {
        using FramePtr = Frame<T, N>*;
        not_null<FramePtr> frame_;

      public:
        FrameHandle() = delete;

        explicit FrameHandle(not_null<FramePtr> frame) noexcept : frame_(frame)
        {
            frame_->refcount.fetch_add(1, std::memory_order_acq_rel);
        }

        FrameHandle(FrameHandle const& other) noexcept : frame_(other.frame_)
        {
            frame_->refcount.fetch_add(1, std::memory_order_acq_rel);
        }

        FrameHandle(FrameHandle&& other) noexcept : frame_(std::exchange(other.frame_, nullptr)) {}

        FrameHandle& operator=(FrameHandle other) noexcept
        {
            std::swap(frame_, other.frame_);
            return *this;
        }

        ~FrameHandle() noexcept
        {
            const int prev = frame_->refcount.fetch_sub(1, std::memory_order_acq_rel);
            assert(prev > 0);
        }

        auto operator*() const noexcept -> Frame<T, N> const& { return *frame_; }
        auto operator->() const noexcept -> not_null<FramePtr> { return frame_; }
    };

} // namespace framelab::channel
