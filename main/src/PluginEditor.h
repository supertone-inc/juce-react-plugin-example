#pragma once

#include "PluginProcessor.h"

#include <juce_gui_extra/juce_gui_extra.h>

//==============================================================================
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &audioProcessor, uint16_t websocketPort);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor &processorRef;

    juce::WebBrowserComponent browser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
