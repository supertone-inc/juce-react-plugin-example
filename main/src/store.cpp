#include "store.h"

#include <juce_dsp/juce_dsp.h>

auto reducer = [](State state, Action action) -> std::pair<State, lager::effect<Action>> {
    if (action["type"].get<std::string>() == ActionType::SET_LEVEL)
    {
        state["level"] = action["payload"];
        return {state, lager::noop};
    }

    if (action["type"].get<std::string>() == ActionType::SET_SPECTRUM)
    {
        state["spectrum"] = action["payload"];
        return {state, lager::noop};
    }

    if (action["type"].get<std::string>() == ActionType::UPDATE_AUDIO_BUFFER)
    {
        return {state, [action = std::move(action)](auto &&ctx) {
                    auto buffer = action["payload"].get<std::vector<float>>();

                    static constexpr auto FFT_ORDER = 10;
                    static constexpr auto FFT_SIZE = 1 << FFT_ORDER;
                    static constexpr auto MIN_DB = -100.0f;
                    static constexpr auto MAX_DB = 0.0f;

                    static juce::dsp::FFT forwardFFT(FFT_ORDER);

                    static std::array<float, FFT_SIZE> fifo;
                    static std::array<float, FFT_SIZE * 2> fftData;
                    static std::array<float, FFT_SIZE / 2> spectrum;

                    static size_t fifoIndex = 0;

                    for (auto sample : buffer)
                    {
                        fifo[fifoIndex++] = sample;

                        if (fifoIndex < FFT_SIZE)
                        {
                            continue;
                        }

                        fifoIndex = 0;

                        std::fill(fftData.begin(), fftData.end(), 0.0f);
                        std::copy(fifo.begin(), fifo.end(), fftData.begin());
                        forwardFFT.performFrequencyOnlyForwardTransform(fftData.data());
                        std::copy(fftData.begin(), fftData.begin() + spectrum.size(), spectrum.begin());

                        for (size_t i = 0; i < spectrum.size(); i++)
                        {
                            spectrum[i] = juce::jmap(juce::jlimit(MIN_DB,
                                                                  MAX_DB,
                                                                  juce::Decibels::gainToDecibels(spectrum[i]) -
                                                                      juce::Decibels::gainToDecibels((float)FFT_SIZE)),
                                                     MIN_DB,
                                                     MAX_DB,
                                                     0.0f,
                                                     1.0f);
                        }

                        auto level = std::accumulate(spectrum.begin(), spectrum.end(), 0.0f) / spectrum.size();

                        ctx.dispatch(Action{{"type", ActionType::SET_SPECTRUM}, {"payload", spectrum}});
                        ctx.dispatch(Action{{"type", ActionType::SET_LEVEL}, {"payload", level}});
                    }
                }};
    }

    return {state, lager::noop};
};

Store createStore(boost::asio::io_context &context)
{
    return lager::make_store<Action>(State(),
                                     lager::with_boost_asio_event_loop{context.get_executor()},
                                     lager::with_reducer(reducer));
}
