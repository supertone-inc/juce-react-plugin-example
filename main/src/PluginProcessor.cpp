#include "PluginProcessor.h"

#include "PluginEditor.h"

#include <juce_dsp/juce_dsp.h>

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         )
    , parameters(*this,
                 nullptr,
                 juce::Identifier("PARAMS"),
                 {
                     std::make_unique<juce::AudioParameterFloat>("gain", // parameterID
                                                                 "Gain", // parameter name
                                                                 0.0f,   // minimum value
                                                                 1.0f,   // maximum value
                                                                 0.75f), // default value
                 })
    , store(createStore(parameters))
{
    for (auto node : parameters.state)
    {
        auto id = node["id"].toString();
        parameters.addParameterListener(id, this);
    }

    store.watch([&](auto state) { webSocketServer.broadcast(state.dump()); });

    webSocketServer.addConnectHandler([&](ClientConnection connection) {
        auto state = store.get();
        webSocketServer.send(connection, state.dump());
    });

    webSocketServer.addMessageHandler([&](ClientConnection connection, const std::string &message) {
        juce::ignoreUnused(connection);
        store.dispatch(json::parse(message));
    });

#if DEBUG
    webSocketServer.start(3001);
#else
    webSocketServer.start(0);
#endif

    DBG("WebSocketServer listening on port " << (int)webSocketServer.getLocalEndpoint().port());
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    static float previousGain = store.get()["parameters"]["gain"];
    float currentGain = store.get()["parameters"]["gain"];

    if (currentGain == previousGain)
    {
        buffer.applyGain(currentGain);
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, currentGain);
        previousGain = currentGain;
    }

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    if (totalNumInputChannels < 1)
    {
        return;
    }

    auto level = buffer.getMagnitude(0, 0, buffer.getNumSamples());

    store.dispatch(Action{ActionType::SET_LEVEL, level});

    static constexpr auto FFT_ORDER = 10;
    static constexpr auto FFT_SIZE = 1 << FFT_ORDER;
    static constexpr auto MIN_DB = -100.0f;
    static constexpr auto MAX_DB = 0.0f;

    static juce::dsp::FFT forwardFFT(FFT_ORDER);

    static std::array<float, FFT_SIZE> fifo;
    static std::array<float, FFT_SIZE * 2> fftData;
    static std::array<float, FFT_SIZE / 2> spectrum;

    static size_t fifoIndex = 0;

    auto *readPointer = buffer.getReadPointer(0);

    for (auto i = 0; i < buffer.getNumSamples(); i++)
    {
        fifo[fifoIndex++] = readPointer[i];

        if (fifoIndex < FFT_SIZE)
        {
            continue;
        }

        fifoIndex = 0;

        std::fill(fftData.begin(), fftData.end(), 0.0f);
        std::copy(fifo.begin(), fifo.end(), fftData.begin());
        forwardFFT.performFrequencyOnlyForwardTransform(fftData.data());
        std::copy(fftData.begin(), fftData.begin() + spectrum.size(), spectrum.begin());

        for (size_t j = 0; j < spectrum.size(); j++)
        {
            spectrum[j] = juce::jmap(juce::jlimit(MIN_DB,
                                                  MAX_DB,
                                                  juce::Decibels::gainToDecibels(spectrum[j]) -
                                                      juce::Decibels::gainToDecibels((float)FFT_SIZE)),
                                     MIN_DB,
                                     MAX_DB,
                                     0.0f,
                                     1.0f);
        }

        store.dispatch(Action{ActionType::SET_SPECTRUM, spectrum});
    }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor(*this, webSocketServer.getLocalEndpoint().port());
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr && xmlState->hasTagName(parameters.state.getType()))
    {
        parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
}

void AudioPluginAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    auto state = store.get();
    auto id = parameterID.toStdString();

    if (abs(newValue - (float)state["parameters"][id]) < 0.001f)
    {
        return;
    }

    store.dispatch(Action{ActionType::UPDATE_PARAMETERS, {{id, newValue}}});
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
