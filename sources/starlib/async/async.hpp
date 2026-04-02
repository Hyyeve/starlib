#pragma once
#include <atomic>
#include <future>
#include "starlib/types/starlib_stdint.hpp"
#include "starlib/types/status.hpp"

namespace starlib::async
{
    ///Guesses an ideal number of threads based on the number of cores in the system, reserving at least two for the main thread and OS.
    u32 ideal_thread_count();

    ///Initializes the threadpool with some number of threads
    status create_threadpool(const u32 num_threads);

    ///Releases the threadpool
    status release_threadpool();

    ///Immediately executes the next task on the calling thread, if any exist.
    ///True if a task was executed, false otherwise
    bool execute_next_task();

    ///Check if the threadpool has any tasks
    bool has_tasks();

    ///Get a numerical ID for the calling thread. Thread IDs are assigned on demand and increment with each known thread.
    u32 thread_id();

    ///Base class for any kind of logic that you want to excecute on the threadpool.
    class task
    {
    public:
        explicit task(const std::initializer_list<task*> dependencies);
        explicit task(const std::vector<task*>& dependencies);

        task() : task({}) {}
        virtual ~task() = default;

        ///Check if the task is completed
        bool completed() const;

        ///Wait for the task to complete, or time out.
        ///If immediate execution is allowed and the task is not blocked, it will run immediately on the calling thread,
        ///which may or may not cause the function to block for longer than the provided timeout.
        ///Returns true if the task was completed, false if the task did not complete.
        bool await_completed(u32 timeout_ms, bool allow_immediate_execution = false);

    protected:
        ///Perform the task logic. Implement this function to create useful tasks!
        virtual void execute() = 0;

    private:
        enum class execution_state : u8
        {
            WAITING, RUNNING, COMPLETED
        };

        friend bool execute_next_task();
        friend void update_waiting_tasks();
        friend void enqueue(task* task);

        bool try_execute_task();
        bool dependencies_completed() const;
        bool state_ready() const;

        std::atomic<execution_state> state = execution_state::WAITING;
        std::vector<task*> depends_on;
        std::promise<void> completion_promise;
        std::future<void> completion_future;
    };
}
