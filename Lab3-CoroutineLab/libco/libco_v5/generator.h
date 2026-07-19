#pragma once

#include <coroutine>
#include <iterator>
#include <utility>

namespace coro {

template <typename Ref, typename Value = std::remove_cvref_t<Ref>>
class generator {
public:
    struct promise_type 
	{
        promise_type* node_;
        promise_type* prev_ = nullptr;
        Value value_;
		std::suspend_always initial_suspend()  { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		std::suspend_always yield_value(Ref&& x) noexcept {
            node_->value_ = std::move(x);
            return {};
        }
        std::suspend_always yield_value(Ref& x) noexcept {
            node_->value_ = x;
            return {};
        }
        void return_void() { return ;}
        generator get_return_object() 
		{
            return generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
		void unhandled_exception() { std::terminate(); }
        promise_type() : node_(this) {}
 
        struct final_awaiter 
		{
            bool await_ready() noexcept { return false;  }

            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) noexcept 
			{
                auto curr = h.promise().node_;
                auto prev = h.promise().prev_;
                if (prev) 
				{
                    curr->node_ = prev;
                    return std::coroutine_handle<promise_type>::from_promise(*prev);
                }
                return std::noop_coroutine();
            }

            void await_resume() noexcept {}
        };
 
 
        struct yield_awaiter 
		{
            generator gen_;

            explicit yield_awaiter(generator &&g) noexcept: gen_(std::move(g)) {}

            bool await_ready() noexcept 
			{
                return !gen_.coro_;
            }

            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) noexcept 
			{
                auto &origin = h.promise();
                auto &nested = gen_.coro_.promise();
                auto &curr = origin.node_;
                nested.node_ = curr;
                curr->node_ = &nested;
                nested.prev_ = &origin;

                return gen_.coro_;
            }

            void await_resume() noexcept {}
        };

        yield_awaiter yield_value(generator&& g) noexcept 
		{
            return yield_awaiter{std::move(g)};
        }

        void resume() 
		{
            std::coroutine_handle<promise_type>::from_promise(*node_).resume();
        }
    };

    generator() noexcept = default;
    
    generator(generator&& other) noexcept: coro_(std::exchange(other.coro_, {})) {}


    ~generator() noexcept {
        if (coro_) 
		{
            coro_.destroy();
        }
    }

    struct sentinel {};

    class iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Value;
        using reference = Ref;
        using pointer = std::add_pointer_t<Ref>;

        iterator() noexcept = default;
        iterator(const iterator&) = delete;
        
        iterator(iterator&& o) {
            std::swap(coro_, o.coro_);
        }

        iterator& operator=(iterator&& o) {
            std::swap(coro_, o.coro_);
            return *this;
        }

        ~iterator() {}

        friend bool operator==(const iterator& o, sentinel) noexcept 
		{
			if(o.coro_ == 0)
			{
				return true;
			}
			if(o.coro_.done())
			{
				return true;
			}
            return false;
        }
                
		friend bool operator!=(const iterator& o, sentinel) noexcept 
		{
			if( o == sentinel{})
			{
				return false;
			}
			return true;
        }

        iterator& operator++() 
		{
            coro_.promise().resume();
            return *this;
        }

        iterator operator++(int) 
		{
            (void) operator++();
        }

        reference operator*() const noexcept 
		{
            return coro_.promise().value_;
        }

        pointer operator->() const noexcept 
		{
            return std::addressof(operator*());
        }


    private:
        friend generator;

        explicit iterator(std::coroutine_handle<promise_type> coro) noexcept : coro_(coro) {}
        std::coroutine_handle<promise_type> coro_ = nullptr;
    };

    iterator begin() noexcept 
	{
        if (coro_) 
		{
            coro_.resume();
        }
        return iterator{coro_};
    }

    sentinel end() noexcept {
        return {};
    }

private:
    explicit generator(std::coroutine_handle<promise_type> h) noexcept : coro_(h) {}
    std::coroutine_handle<promise_type> coro_ = nullptr;
};

}  // namespace coro
