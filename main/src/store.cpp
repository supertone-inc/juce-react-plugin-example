#include "store.h"

#include <juce_dsp/juce_dsp.h>

auto reducer = [](State state, Action action) {
    if (action["type"].get<std::string>() == ActionType::UPDATE_AUDIO_SAMPLE)
    {
        static constexpr auto FFT_ORDER = 10;
        static constexpr auto FFT_SIZE = 1 << FFT_ORDER;
        static constexpr auto MIN_DB = -100.0f;
        static constexpr auto MAX_DB = 0.0f;

        static juce::dsp::FFT forwardFFT(FFT_ORDER);

        static std::array<float, FFT_SIZE> fifo;
        static std::array<float, FFT_SIZE * 2> fftData;
        static std::array<float, FFT_SIZE / 2> spectrum;

        static size_t fifoIndex = 0;

        auto sample = action["payload"].get<float>();

        fifo[fifoIndex++] = sample;

        if (fifoIndex < FFT_SIZE)
        {
            return state;
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

        state["spectrum"] = spectrum;
        state["level"] = std::accumulate(spectrum.begin(), spectrum.end(), 0.0f) / spectrum.size();

        return state;
    }

    return state;
};

Store create_store()
{
    return redux::create_store(reducer);
}
