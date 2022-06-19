/**
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#pragma once

#include <mutex>

namespace srf::internal {

enum class ServiceState
{
    Initialized,
    Running,
    Stopping,
    Killing,
    Awaiting,
    Completed,
};

// struct IService
// {
//     virtual ~IService() = default;

//     virtual void service_start()      = 0;
//     virtual void service_await_live() = 0;
//     virtual void service_stop()       = 0;
//     virtual void service_kill()       = 0;
//     virtual void service_await_join() = 0;
// };

class Service  // : public IService
{
  public:
    virtual ~Service();

    void service_start();
    void service_await_live();
    void service_stop();
    void service_kill();
    void service_await_join();

    const ServiceState& state() const;

  protected:
    void call_in_destructor();

  private:
    bool forward_state(ServiceState new_state);

    virtual void do_service_start()      = 0;
    virtual void do_service_await_live() = 0;
    virtual void do_service_stop()       = 0;
    virtual void do_service_kill()       = 0;
    virtual void do_service_await_join() = 0;

    ServiceState m_state{ServiceState::Initialized};
    mutable std::mutex m_mutex;
};

}  // namespace srf::internal
