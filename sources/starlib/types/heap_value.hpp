#pragma once
#include <functional>
#include <memory>
#include <utility>

namespace starlib
{
    //Wrapper over a heap-allocated value that can be used with copy semantics
    template <typename wrapped_type>
    class heap_value
    {
    public:
        // ReSharper disable once CppNonExplicitConvertingConstructor
        heap_value(wrapped_type&& value) : value(std::move(value)) {}
        // ReSharper disable once CppNonExplicitConvertingConstructor
        heap_value(wrapped_type*&& value) : value(std::move(value)) {}

        heap_value(const heap_value& other)
        {
            value = new wrapped_type(*other.value);
        }

        heap_value(heap_value& other) : heap_value(static_cast<const heap_value&>(other)) {}
        heap_value(heap_value&& other) noexcept
        {
            value = std::move(other.value);
        }

        heap_value& operator=(const heap_value& other)
        {
            delete value;
            value = new wrapped_type(other.value);
            return *this;
        }

        heap_value& operator=(heap_value&& other) noexcept
        {
            delete value;
            value = std::move(other.value);
            return *this;
        }

        const wrapped_type& operator*() const
        {
            return *ptr();
        }

        wrapped_type& operator*()
        {
            return *ptr();
        }

        const wrapped_type* operator->() const
        {
            return ptr();
        }

        wrapped_type* operator->()
        {
            return ptr();
        }

        bool operator==(const heap_value& other) const
        {
            if (ptr() == nullptr || other.ptr() == nullptr) return ptr() == other.ptr();
            return *ptr() == *other.ptr();
        }

        const wrapped_type* ptr() const
        {
            return value;
        }

        wrapped_type* ptr()
        {
            return value;
        }

    private:
        wrapped_type* value = nullptr;
    };
}
