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

#include "../nodes/common_nodes.hpp"

#include "srf/channel/status.hpp"
#include "srf/node/rx_sink.hpp"
#include "srf/node/rx_source.hpp"
#include "srf/segment/builder.hpp"
#include "srf/segment/definition.hpp"
#include "srf/segment/object.hpp"

#include "rxcpp/rx-includes.hpp"
#include "rxcpp/rx-observer.hpp"
#include "rxcpp/rx-operators.hpp"
#include "rxcpp/rx-predef.hpp"
#include "rxcpp/rx-subscriber.hpp"

#include <memory>
#include <string>
#include <vector>

using namespace srf;

namespace test::segments {

std::shared_ptr<segment::Definition> single_finite_no_ports(std::string segment_name)
{
    return segment::Definition::create(segment_name, [](segment::Builder& s) {
        auto rx_source = s.make_object("rx_source", test::nodes::finite_int_rx_source());
        auto rx_sink   = s.make_object("rx_sink", test::nodes::int_sink());
        s.make_edge(rx_source, rx_sink);
    });
}

std::shared_ptr<segment::Definition> single_finite_no_ports_will_throw(std::string segment_name)
{
    return segment::Definition::create(segment_name, [](segment::Builder& s) {
        auto rx_source = s.make_object("rx_source", test::nodes::finite_int_rx_source());
        auto rx_sink   = s.make_object("rx_sink", test::nodes::int_sink_throw_on_even());
        s.make_edge(rx_source, rx_sink);
    });
}

}  // namespace test::segments
