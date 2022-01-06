#pragma once

#include <lager/event_loop/boost_asio.hpp>
#include <lager/store.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using State = json;
using Action = json;
using Store = lager::store<Action, State>;

namespace ActionType
{
const std::string SET_LEVEL("SET_LEVEL");
const std::string SET_SPECTRUM("SET_SPECTRUM");
const std::string UPDATE_AUDIO_BUFFER("UPDATE_AUDIO_BUFFER");
} // namespace ActionType

Store create_store(boost::asio::io_context &context);
