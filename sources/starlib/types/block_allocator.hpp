#pragma once
#include <list>
#include "starlib_stdint.hpp"

namespace starlib
{
    ///A simple allocation tracker for all of your 'keeping track of sized blocks that can't overlap' needs.
    class block_allocator
    {
    public:
        explicit block_allocator(const u64 size);

        [[nodiscard]] bool can_allocate(const u64 size) const;
        [[nodiscard]] bool try_allocate(const u64 size, u64& address_out);
        void free(const u64 address);
        void resize(const u64 size);
        void clear();

        [[nodiscard]] u64 last_used_address() const;
    private:

        [[nodiscard]] bool find_free(const u64 size, u64& start, u64& block_idx, bool& is_end_allocation) const;

        struct block
        {
            u64 start;
            u64 size;
        };

        std::list<block> allocations {};
        u64 total_pool_size;
        u64 remaining_available_size;


    };
}
