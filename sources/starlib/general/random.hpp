#pragma once
#include <random>

#include "pcg/pcg_random.hpp"
#include "starlib/types/starlib_stdint.hpp"

namespace starlib
{
    typedef pcg::pcg32_fast random32;
    typedef pcg::pcg64_fast random64;

    inline random32 seeded_random32(const starlib_stdint::u64 seed)
    {
        return random32(std::seed_seq {seed, 328721 + seed * 17, 798823 + (seed << 6), seed - 325232, 3974 * seed ^ seed >> 1});
    }

    inline random64 seeded_random64(const starlib_stdint::u64 seed)
    {
        return random64(std::seed_seq {seed, 987379 + seed * 19, 239789 + (seed << 2), seed - 817283, 8921 * seed ^ seed >> 9});
    }
}