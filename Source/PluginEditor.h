/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AttackonPianoAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AttackonPianoAudioProcessorEditor (AttackonPianoAudioProcessor&);
    ~AttackonPianoAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void setMidiInput(int index);

private:
    juce::TextButton mLoadButton{ "Loady" };
    AttackonPianoAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttackonPianoAudioProcessorEditor)
};