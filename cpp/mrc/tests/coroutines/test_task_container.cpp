/**
 * SPDX-FileCopyrightText: Copyright (c) 2022-2023, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mrc/coroutines/sync_wait.hpp"
#include "mrc/coroutines/task.hpp"
#include "mrc/coroutines/task_container.hpp"
#include "mrc/coroutines/test_scheduler.hpp"

#include <gtest/gtest.h>

#include <chrono>     // for time_point, duration
#include <coroutine>  // for coroutine_handle, suspe...
#include <memory>
#include <ratio>  // for ratio
#include <thread>
#include <vector>  // for vector

class TestCoroTaskContainer : public ::testing::Test
{};

TEST_F(TestCoroTaskContainer, LifeCycle) {}

TEST_F(TestCoroTaskContainer, MaxSimultaneousTasks)
{
    using namespace std::chrono_literals;

    auto on             = std::make_shared<mrc::coroutines::TestScheduler>();
    auto task_container = mrc::coroutines::TaskContainer(on, 2);

    auto start_time = on->time();

    std::vector<std::chrono::time_point<std::chrono::steady_clock>> execution_times;

    auto delay = [](std::shared_ptr<mrc::coroutines::TestScheduler> on,
                    std::vector<std::chrono::time_point<std::chrono::steady_clock>>& execution_times)
        -> mrc::coroutines::Task<> {
        co_await on->yield_for(100ms);
        execution_times.emplace_back(on->time());
    };

    std::vector<std::thread> threads;

    for (auto i = 0; i < 4; i++)
    {
        threads.emplace_back([&]() {
            for (auto i = 0; i < 4; i++)
            {
                task_container.start(delay(on, execution_times));
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    auto task = task_container.garbage_collect_and_yield_until_empty();

    task.resume();

    while (on->resume_next()) {}

    mrc::coroutines::sync_wait(task);

    ASSERT_EQ(execution_times.size(), threads.size() * 4);

    for (auto i = 0; i < execution_times.size(); i++)
    {
        ASSERT_EQ(execution_times[i], start_time + (i / 2 + 1) * 100ms);
    }
}
