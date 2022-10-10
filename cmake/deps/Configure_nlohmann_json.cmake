#=============================================================================
# SPDX-FileCopyrightText: Copyright (c) 2020-2022, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#=============================================================================

function(find_and_configure_nlohmann_json VERSION)

  list(APPEND CMAKE_MESSAGE_CONTEXT "nlohmann_json")

  rapids_cpm_find(nlohmann_json ${VERSION}
    GLOBAL_TARGETS
      nlohmann_json nlohmann_json::nlohmann_json
    BUILD_EXPORT_SET
      ${PROJECT_NAME}-core-exports
    INSTALL_EXPORT_SET
      ${PROJECT_NAME}-core-exports
    CPM_ARGS
      GIT_REPOSITORY          https://github.com/nlohmann/json.git
      GIT_TAG                 ${GIT_TAG}
      GIT_SHALLOW             TRUE
      OPTIONS                 "JSON_Install ON"
  )

  if (nlohmann_json_ADDED)
    target_include_directories(nlohmann_json
        INTERFACE $<BUILD_INTERFACE:${nlohmann_json_SOURCE_DIR}/include>
                  $<BUILD_INTERFACE:${nlohmann_json_BINARY_DIR}/include>
    )
  endif()

endfunction()

find_and_configure_nlohmann_json("3.9.1" "v3.9.1")