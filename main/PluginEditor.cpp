#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &audioProcessor,
                                                                 uint16_t websocketPort)
    : AudioProcessorEditor(&audioProcessor), processorRef(audioProcessor)
{
    juce::ignoreUnused(processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);

    addAndMakeVisible(browser);

#if DEBUG
    auto url = juce::URL("http://localhost:3000");
#else
    auto currentExecutableDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getParentDirectory();
    auto file = currentExecutableDir.getChildFile("renderer").getChildFile("index.html");
    auto url = juce::URL(file);
#endif

    url = url.withParameter("port", juce::String(websocketPort));

    browser.goToURL(url.toString(true));
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g)
{
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    browser.setBounds(0, 0, getWidth(), getHeight());
}
