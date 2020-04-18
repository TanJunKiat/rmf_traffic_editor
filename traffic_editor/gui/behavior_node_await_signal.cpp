/*
 * Copyright (C) 2019-2020 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <algorithm>
#include "traffic_editor/behavior_node_await_signal.h"

using std::string;
using std::make_unique;


BehaviorNodeAwaitSignal::BehaviorNodeAwaitSignal(const YAML::Node& y)
: BehaviorNode()
{
  signal_name = y[1].as<string>();
}

BehaviorNodeAwaitSignal::~BehaviorNodeAwaitSignal()
{
}

void BehaviorNodeAwaitSignal::print() const
{
  printf("      await_signal: %s\n", signal_name.c_str());
}

void BehaviorNodeAwaitSignal::tick(
    const double /*dt_seconds*/,
    ModelState& /*state*/,
    Building& /*building*/,
    const std::vector<std::string>& inbound_messages,
    std::vector<std::string>& /*outbound_messages*/)
{
  if (std::find(inbound_messages.begin(),
          inbound_messages.end(),
          signal_name)
      != inbound_messages.end())
    signal_received = true;
}

bool BehaviorNodeAwaitSignal::is_complete() const
{
  return signal_name.empty() || signal_received;
}
