#pragma once
#include <list>
#include <mutex>
#include <thread>
#include <vector>

#include "../general/stdint.hpp"

namespace starlib
{
    class threadpool_task;

    class threadpool
    {
    public:
        ///Initializes the threadpool with some number of threads
        explicit threadpool(u32 thread_count);

        ///Frees the threadpool. Will block until all threads in the pool finish their current tasks and exit.
        ~threadpool();

        ///Guesses an ideal number of threads based on the number of cores in the system, reserving at least two for the main thread and OS.
        static u32 ideal_thread_count();

        ///Immediately executes the next task on the calling thread, if any exist.
        ///True if a task was executed, false otherwise
        bool execute_next_task();

        ///Check if the threadpool has any tasks
        bool has_tasks();

        void enqueue(threadpool_task* task);

    private:
        void thread_loop();
        void update_waiting_tasks();

        std::vector<std::thread> threads;

        std::mutex work_lock;
        std::condition_variable work_condition;

        bool threadpool_exiting;
        std::vector<threadpool_task*> waiting_tasks; //Tasks that cannot be executed yet due to waiting on dependencies
        std::list<threadpool_task*> ready_tasks;     //Tasks that are not waiting on dependencies and can be dequeued
    };
}
