#include "store.h"

using ReducerResult = std::pair<State, lager::effect<Action, lager::deps<juce::AudioProcessorValueTreeState &>>>;

auto reducer = [](State state, Action action) -> ReducerResult {
    if (action.type == ActionType::SET_LEVEL)
    {
        state["level"] = action.payload;
        return {state, lager::noop};
    }

    if (action.type == ActionType::SET_SPECTRUM)
    {
        state["spectrum"] = action.payload;
        return {state, lager::noop};
    }

    if (action.type == ActionType::UPDATE_PARAMETERS)
    {
        for (auto &[key, value] : action.payload.items())
        {
            state["parameters"][key] = value;
        }

        return {state, [action = std::move(action)](auto &&ctx) {
                    auto &parameters = lager::get<juce::AudioProcessorValueTreeState>(ctx);

                    for (auto &[key, value] : action.payload.items())
                    {
                        parameters.getParameter(key)->setValueNotifyingHost(value);
                    }
                }};
    }

    return {state, lager::noop};
};

struct with_juce_event_dispatch_loop
{
    template <typename Fn>
    void async(Fn &&)
    {
        throw std::logic_error{"juce_event_dispatch_loop does not support async()"};
    }

    template <typename Fn>
    void post(Fn &&fn)
    {
        juce::MessageManager::callAsync(std::forward<Fn>(fn));
    }

    void finish()
    {
    }

    void pause()
    {
    }

    void resume()
    {
    }
};

Store createStore(juce::AudioProcessorValueTreeState &parameters)
{
    auto initialState = State();

    for (auto node : parameters.state)
    {
        std::string key = node["id"].toString().toStdString();
        float value = node["value"];

        initialState["parameters"][key] = value;
    }

    return lager::make_store<Action>(initialState,
                                     with_juce_event_dispatch_loop{},
                                     lager::with_reducer(reducer),
                                     lager::with_deps(std::ref(parameters)));
}
