#include "async.hpp"

#include <atomic>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <unordered_set>

#include "nanotime/nanotime.hpp"
#include "tracy/Tracy.hpp"

namespace starlib
{
    static atomic_u32 thread_id_counter = 0;
    static std::unordered_map<u32, u32> thread_id_map;
    static std::mutex thread_id_lock;

    u32 thread_id()
    {
        thread_id_lock.lock();
        u32 id = 0;
        const u32 hash = std::hash<std::thread::id> {}(std::this_thread::get_id());
        if (thread_id_map.contains(hash))
        {
            id = thread_id_map[hash];
        }
        else
        {
            id = thread_id_counter;
            thread_id_counter += 1;
            thread_id_map[hash] = id;
        }
        thread_id_lock.unlock();

        return id;
    }

    void sleep_nanos(const u64 nanoseconds)
    {
        ZoneScoped;
        nanotime::nanotime_sleep(nanoseconds);
    }

    void sleep_micros(const u64 microseconds)
    {
        sleep_nanos(microseconds * 1000);
    }

}
