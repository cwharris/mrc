#pragma once

#include "pymrc/coro.hpp"
#include "pymrc/utilities/acquire_gil.hpp"

#include <boost/fiber/future/async.hpp>
#include <mrc/coroutines/scheduler.hpp>
#include <mrc/coroutines/task.hpp>
#include <mrc/coroutines/task_container.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace mrc::pymrc {
class AsyncioScheduler : public mrc::coroutines::Scheduler
{
  public:
    AsyncioScheduler(size_t concurrency) {}

    std::string description() const override
    {
        return "AsyncioScheduler";
    }

    void resume(std::coroutine_handle<> coroutine) override
    {
        if (coroutine.done())
        {
            LOG(WARNING) << "AsyncioScheduler::resume() > Attempted to resume a completed coroutine";
            return;
        }

        py::gil_scoped_acquire gil;

        auto& loop = this->get_loop();

        // TODO(MDD): Check whether or not we need thread safe version
        loop.attr("call_soon_threadsafe")(py::cpp_function([this, handle = std::move(coroutine)]() {
            if (handle.done())
            {
                LOG(WARNING) << "AsyncioScheduler::resume() > Attempted to resume a completed coroutine";
                return;
            }

            py::gil_scoped_release nogil;

            handle.resume();
        }));
    }

    mrc::pymrc::PyHolder& init_loop()
    {
        CHECK_EQ(PyGILState_Check(), 1) << "Must have the GIL when calling AsyncioScheduler::init_loop()";

        std::unique_lock lock(m_mutex);

        if (m_loop)
        {
            return m_loop;
        }

        auto asyncio_mod = py::module_::import("asyncio");

        py::object loop;

        try
        {
            // Otherwise check if one is already allocated
            loop = asyncio_mod.attr("get_running_loop")();
        } catch (std::runtime_error&)
        {
            // Need to create a loop
            LOG(INFO) << "AsyncioScheduler::run() > Creating new event loop";

            // Gets (or more likely, creates) an event loop and runs it forever until stop is called
            loop = asyncio_mod.attr("new_event_loop")();

            // Set the event loop as the current event loop
            asyncio_mod.attr("set_event_loop")(loop);
        }

        m_loop = std::move(loop);

        return m_loop;
    }

    // Runs the task until its complete
    void run_until_complete(coroutines::Task<>&& task)
    {
        mrc::pymrc::AcquireGIL gil;

        auto& loop = this->init_loop();

        LOG(INFO) << "AsyncioScheduler::run() > Calling run_until_complete() on main_task()";

        // Use the BoostFibersMainPyAwaitable to allow fibers to be progressed
        loop.attr("run_until_complete")(mrc::pymrc::coro::BoostFibersMainPyAwaitable(std::move(task)));
    }

  private:
    std::coroutine_handle<> schedule_operation(Operation* operation) override
    {
        this->resume(std::move(operation->m_awaiting_coroutine));

        return std::noop_coroutine();
    }

    mrc::pymrc::PyHolder& get_loop()
    {
        if (!m_loop)
        {
            throw std::runtime_error("Must call init_loop() before get_loop()");
        }

        // TODO(MDD): Check that we are on the same thread as the loop
        return m_loop;
    }

    std::mutex m_mutex;

    std::atomic_size_t m_outstanding{0};

    mrc::pymrc::PyHolder m_loop;
};

}  // namespace mrc::pymrc