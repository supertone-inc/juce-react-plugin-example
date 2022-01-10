#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
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
const std::string UPDATE_PARAMETERS("UPDATE_PARAMETERS");
} // namespace ActionType

Store createStore(boost::asio::io_context &context, juce::AudioProcessorValueTreeState &parameters);
