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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AttackonPianoAudioProcessor& audioProcessor;

    juce::Slider midiVolume; // [1]
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;

    //add an option to present a list of MIDI inputs
    juce::ComboBox midiInputList;
    juce::Label midiInputListLabel;
    int lastInputIndex = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttackonPianoAudioProcessorEditor)
};
