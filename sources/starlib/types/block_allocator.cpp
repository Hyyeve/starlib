#include "block_allocator.hpp"
#include <tracy/Tracy.hpp>

namespace starlib
{
    block_allocator::block_allocator(const u64 size) : total_pool_size(size), remaining_available_size(total_pool_size) {}

    bool block_allocator::can_allocate(const u64 size) const
    {
        ZoneScoped;
        u64 block_idx;
        bool is_end_allocation;
        u64 address_out;
        return find_free(size, address_out, block_idx, is_end_allocation);
    }

    bool block_allocator::try_allocate(const u64 size, u64& address_out)
    {
        ZoneScoped;
        u64 block_idx;
        bool is_end_allocation;
        const bool has_space = find_free(size, address_out, block_idx, is_end_allocation);

        if (!has_space) return false;

        if (is_end_allocation) allocations.push_back({address_out, size});
        else allocations.insert(std::next(allocations.begin(), block_idx), {address_out, size});

        remaining_available_size -= size;
        return true;
    }

    void block_allocator::free(const u64 address)
    {
        ZoneScoped;
        u64 size = 0;
        const auto loc = std::ranges::find_if(allocations, [address, &size](const block& block)
        {
            if (block.start == address)
            {
                size = block.size;
                return true;
            }
            return false;
        });

        if (loc != allocations.end())
        {
            remaining_available_size += size;
            allocations.erase(loc);
        }
    }

    void block_allocator::resize(const u64 size)
    {
        remaining_available_size += size - total_pool_size;
        total_pool_size = size;
    }

    void block_allocator::clear()
    {
        allocations.clear();
        remaining_available_size = total_pool_size;
    }

    u64 block_allocator::last_used_address() const
    {
        if (allocations.empty()) return 0;
        const block& last_block = allocations.back();
        return last_block.start + last_block.size;
    }

    bool block_allocator::find_free(const u64 size, u64& start, u64& block_idx, bool& is_end_allocation) const
    {
        ZoneScoped;

        //Can't fit!
        if (size > remaining_available_size) return false;

        //Definitely fits
        if (allocations.empty())
        {
            start = 0;
            block_idx = 0;
            return true;
        }

        //Might fit at the end of the pool?
        const block& last_block = allocations.back();
        start = last_block.start + last_block.size;
        if (start + size <= total_pool_size)
        {
            block_idx = allocations.size();
            is_end_allocation = true;
            return true;
        }

        //Doesn't fit at the end - do we have space elsewhere?
        u64 remaining_possible_space = remaining_available_size - last_block.size;
        if (size > remaining_possible_space) return false;

        //Might have an empty space big enough from deallocations - iterate through and check
        //Performs best if elements are allocated and deallocated in roughly FIFO order
        start = 0;
        block_idx = 0;
        is_end_allocation = false;
        for (const block block : allocations)
        {
            //Does the next block overlap the space we need?
            if (block.start < start + size)
            {
                //If so, move our start to the end of it, and update the amount of space that's left to check
                remaining_possible_space -= block.size;
                start = block.start + block.size;
                block_idx++;
            }
            else
            {
                //If not, we have space here!
                return true;
            }

            //There can't be enough space left after what we've checked so far, so we immediately know we can't allocate.
            if (size > remaining_possible_space) return false;
        }

        return false;
    }
}
