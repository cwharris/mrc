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

#include <srf/types.hpp>

#include <ucp/api/ucp.h>
#include <boost/fiber/all.hpp>

#include <memory>
#include <string>

namespace srf::internal::ucx {

using WorkerAddress = std::string;  // NOLINT

using InstanceID = std::uint32_t;  // NOLINT
using NodeID     = std::uint64_t;  // NOLINT
using ObjectID   = std::uint64_t;  // NOLINT

template <typename T>
class Primitive;

class Context;
class Worker;
class Endpoint;

class SendManager;
class ReceiveManager;

}  // namespace srf::internal::ucx
