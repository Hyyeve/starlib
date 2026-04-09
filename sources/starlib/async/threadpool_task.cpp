#include "threadpool_task.hpp"
#include "tracy/Tracy.hpp"

namespace starlib
{
    threadpool_task::threadpool_task(const std::initializer_list<threadpool_task*> dependencies) : depends_on(dependencies)
    {
        completion_promise = std::promise<void>();
        completion_future = completion_promise.get_future();
    }

    threadpool_task::threadpool_task(const std::vector<threadpool_task*>& dependencies) : depends_on(dependencies)
    {
        completion_promise = std::promise<void>();
        completion_future = completion_promise.get_future();
    }

    bool threadpool_task::completed() const
    {
        return state == execution_state::COMPLETED;
    }

    bool threadpool_task::await_completed(const u32 timeout_ms, const bool allow_immediate_execution)
    {
        ZoneScoped;
        if (allow_immediate_execution && try_execute_task()) return true;
        completion_future.wait_for(std::chrono::milliseconds(timeout_ms));
        return completed();
    }

    bool threadpool_task::try_execute_task()
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

    bool threadpool_task::dependencies_completed() const
    {
        for (const threadpool_task* dependent : depends_on)
        {
            if (!dependent->completed()) return false;
        }
        return true;
    }

    bool threadpool_task::state_ready() const
    {
        return state == execution_state::WAITING;
    }
}
