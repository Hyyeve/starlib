#include "threadpool.hpp"

#include "threadpool_task.hpp"
#include "tracy/Tracy.hpp"

namespace starlib
{
    threadpool::threadpool(const u32 thread_count)
    {
        ZoneScoped;
        work_lock.lock();

        threadpool_exiting = false;

        threads.reserve(thread_count);
        for (u32 i = 0; i < thread_count; ++i)
        {
            threads.emplace_back([i, this]
            {
                #if TRACY_ENABLE
                // Tracy requires memory passed to the profiler to be pinned and never unallocated.
                // ReSharper disable once CppDFAMemoryLeak
                char* thread_name = new char[16];
                snprintf(thread_name, 16, " Worker %i ", i);
                tracy::SetThreadName(thread_name);
                #endif

                thread_loop();
            });
        }

        work_lock.unlock();
    }

    threadpool::~threadpool()
    {
        ZoneScoped;

        work_lock.lock();
        threadpool_exiting = true;
        work_lock.unlock();

        work_condition.notify_all();

        for (std::thread& thread : threads)
        {
            thread.join();
        }

        threads.clear();
    }

    u32 threadpool::ideal_thread_count()
    {
        const u32 hardware_num = std::thread::hardware_concurrency(); //Number of logical cores availble
        if (hardware_num == 0) return 4; //We don't know how many to use, just suggest 4.
        if (hardware_num > 2) return hardware_num - 2; //Use all available cores but leave 2 free for main thread and OS
        return 1; //Not enough cores, but we always want to make at least one.
    }

    bool threadpool::execute_next_task()
    {
        ZoneScoped;
        threadpool_task* next_task = nullptr;
        work_lock.lock();
        if (!ready_tasks.empty())
        {
            next_task = ready_tasks.front();
            ready_tasks.pop_front();
        }
        work_lock.unlock();

        if (next_task == nullptr)
        {
            return false;
        }

        next_task->try_execute_task();
        update_waiting_tasks();

        return true;
    }

    bool threadpool::has_tasks()
    {
        bool result = true;
        work_lock.lock();
        if (ready_tasks.empty()) result = false;
        work_lock.unlock();
        return result;
    }

    void threadpool::enqueue(threadpool_task* task)
    {
        if (!task->state_ready()) return; //Task already running or completed

        work_lock.lock();
        const bool ready = task->dependencies_completed();

        if (ready) ready_tasks.push_back(task);
        else waiting_tasks.push_back(task);

        work_lock.unlock();
        work_condition.notify_one();
    }

    void threadpool::thread_loop()
    {
        ZoneScoped;
        while (true)
        {
            const bool did_execute_task = execute_next_task();
            if (did_execute_task) continue; //work until no more tasks available

            //Exit / work conditions could be changed here; thread would block until changes written

            std::unique_lock lock(work_lock); //block until we get the lock

            //Exit/work conditions can't be changed here - would block and wait for this thread to check them

            if (threadpool_exiting) break; //check if we should exit
            if (ready_tasks.empty())        //check if a task has been added since we finished the last one to prevent deadlock
            {
                work_condition.wait(lock); //Lock released here, exit/work conditions can be changed and thread will see notifications
            }

            //Lock is re-locked here by waking thread

            lock.unlock(); //Release the lock while we do work
        }
    }

    void threadpool::update_waiting_tasks()
    {
        ZoneScoped;
        work_lock.lock();
        erase_if(waiting_tasks, [this](threadpool_task* task)
        {
            const bool is_ready = task->dependencies_completed();
            if (is_ready) ready_tasks.push_back(task);
            return is_ready;
        });
        work_lock.unlock();
    }
}
