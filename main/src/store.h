#pragma once

#include <nlohmann/json.hpp>
#include <redux/redux.hpp>

using json = nlohmann::json;

using State = json;
using Action = json;
using Store = redux::Store<State, Action>;

namespace ActionType
{
const std::string UPDATE_AUDIO_SAMPLE("UPDATE_AUDIO_SAMPLE");
}

Store create_store();
