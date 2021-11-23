#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible(browser);

#if DEBUG
    browser.goToURL("http://localhost:3000");
#else
    auto currentExecutableDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getParentDirectory();
    auto file = currentExecutableDir.getChildFile("renderer").getChildFile("index.html");
    auto url = juce::URL(file);

    browser.goToURL(url.toString(true));
#endif
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    browser.setBounds(0, 0, getWidth(), getHeight());
}