#include "async.hpp"

#include <atomic>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <unordered_set>
#include "tracy/Tracy.hpp"

namespace starlib::async
{
    static atomic_u32 thread_id_counter = 0;
    static std::unordered_map<u32, u32> thread_id_map;
    static std::mutex thread_id_lock;

    static std::vector<std::thread> threads;

    static std::mutex threadpool_work_lock;
    static std::condition_variable threadpool_work_condition;

    static bool should_threads_exit;
    static std::vector<task*> waiting_tasks; //Tasks that cannot be executed yet due to waiting on dependencies
    static std::list<task*> ready_tasks;     //Tasks that are not waiting on dependencies and can be dequeued

    void thread_loop()
    {
        ZoneScoped;
        while (true)
        {
            const bool did_execute_task = execute_next_task();
            if (did_execute_task) continue; //work until no more tasks available

            //Exit / work conditions could be changed here; thread would block until changes written

            std::unique_lock lock(threadpool_work_lock); //block until we get the lock

            //Exit/work conditions can't be changed here - would block and wait for this thread to check them

            if (should_threads_exit) break; //check if we should exit
            if (ready_tasks.empty())        //check if a task has been added since we finished the last one to prevent deadlock
            {
                threadpool_work_condition.wait(lock); //Lock released here, exit/work conditions can be changed and thread will see notifications
            }

            //Lock is re-locked here by waking thread

            lock.unlock(); //Release the lock while we do work
        }
    }

    u32 ideal_thread_count()
    {
        //We want one task thread per logical core, leaving two cores free for the main thread and OS
        //If there isn't any logical cores available (or the hardware concurrency hint is unavailable) we just make 3.
        const i32 available_logical_cores = std::thread::hardware_concurrency() - 2;
        return static_cast<u32>(std::max(available_logical_cores, 3));
    }

    status create_threadpool(const u32 num_threads)
    {
        ZoneScoped;
        threadpool_work_lock.lock();

        if (!threads.empty())
        {
            u32 size = threads.size();
            threadpool_work_lock.unlock();
            return {status_type::ALREADY_INITIALIZED, std::format("Already created threadpool, has {0} threads", size)};
        }

        should_threads_exit = false;

        threads.reserve(num_threads);
        for (u32 i = 0; i < num_threads; ++i)
        {
            threads.emplace_back([i]
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

        threadpool_work_lock.unlock();
        return status_type::SUCCESS;
    }

    status release_threadpool()
    {
        ZoneScoped;

        threadpool_work_lock.lock();
        should_threads_exit = true;
        threadpool_work_lock.unlock();

        threadpool_work_condition.notify_all();

        for (std::thread& thread : threads)
        {
            thread.join();
        }

        threads.clear();
        return status_type::SUCCESS;
    }

    void update_waiting_tasks()
    {
        ZoneScoped;
        threadpool_work_lock.lock();
        erase_if(waiting_tasks, [](task* task)
        {
            const bool is_ready = task->dependencies_completed();
            if (is_ready) ready_tasks.push_back(task);
            return is_ready;
        });
        threadpool_work_lock.unlock();
    }

    bool execute_next_task()
    {
        ZoneScoped;
        task* next_task = nullptr;
        threadpool_work_lock.lock();
        if (!ready_tasks.empty())
        {
            next_task = ready_tasks.front();
            ready_tasks.pop_front();
        }
        threadpool_work_lock.unlock();

        if (next_task == nullptr)
        {
            return false;
        }

        next_task->try_execute_task();
        update_waiting_tasks();

        return true;
    }

    bool has_tasks()
    {
        bool result = true;
        threadpool_work_lock.lock();
        if (ready_tasks.empty()) result = false;
        threadpool_work_lock.unlock();
        return result;
    }

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

    task::task(const std::initializer_list<task*> dependencies) : depends_on(dependencies)
    {
        completion_promise = std::promise<void>();
        completion_future = completion_promise.get_future();
    }

    task::task(const std::vector<task*>& dependencies) : depends_on(dependencies)
    {
        completion_promise = std::promise<void>();
        completion_future = completion_promise.get_future();
    }

    bool task::completed() const
    {
        return state == execution_state::COMPLETED;
    }

    bool task::await_completed(const u32 timeout_ms, const bool allow_immediate_execution)
    {
        ZoneScoped;
        if (allow_immediate_execution && try_execute_task()) return true;
        completion_future.wait_for(std::chrono::milliseconds(timeout_ms));
        return completed();
    }

    void enqueue(task* task)
    {
        if (!task->state_ready()) return; //Task already running or completed

        threadpool_work_lock.lock();
        const bool ready = task->dependencies_completed();

        if (ready) ready_tasks.push_back(task);
        else waiting_tasks.push_back(task);

        threadpool_work_lock.unlock();
        threadpool_work_condition.notify_one();
    }

    bool task::try_execute_task()
    {
        ZoneScoped;
        if (dependencies_completed() && state_ready())
        {
            state = execution_state::RUNNING;
            execute();
            completion_promise.set_value();
            state = execution_state::COMPLETED;
            return true;
        }

        return false;
    }

    bool task::dependencies_completed() const
    {
        for (const task* dependent : depends_on)
        {
            if (!dependent->completed()) return false;
        }
        return true;
    }

    bool task::state_ready() const
    {
        return state == execution_state::WAITING;
    }
}
