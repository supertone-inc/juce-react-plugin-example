#pragma once

#include <lager/store.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using State = json;
using Action = json;
using Store = lager::store<Action, State>;

namespace ActionType
{
const std::string UPDATE_AUDIO_SAMPLE("UPDATE_AUDIO_SAMPLE");
}

Store create_store();
