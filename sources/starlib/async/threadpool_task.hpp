#pragma once
#include <future>
#include <initializer_list>
#include <vector>

#include "../general/starlib_stdint.hpp"

namespace starlib
{
    ///Base class for any kind of logic that you want to excecute on a threadpool.
    class threadpool_task
    {
    public:
        explicit threadpool_task(const std::initializer_list<threadpool_task*> dependencies);
        explicit threadpool_task(const std::vector<threadpool_task*>& dependencies);

        threadpool_task() : threadpool_task({}) {}
        virtual ~threadpool_task() = default;

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
        friend class threadpool;

        enum class execution_state : u8
        {
            WAITING, RUNNING, COMPLETED
        };

        bool try_execute_task();
        bool dependencies_completed() const;
        bool state_ready() const;

        std::atomic<execution_state> state = execution_state::WAITING;
        std::vector<threadpool_task*> depends_on;
        std::promise<void> completion_promise;
        std::future<void> completion_future;
    };
}
