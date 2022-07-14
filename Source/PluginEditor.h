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
class AttackonPianoAudioProcessorEditor  : public juce::AudioProcessorEditor, 
                                           public juce::FileDragAndDropTarget
{
public:
    AttackonPianoAudioProcessorEditor (AttackonPianoAudioProcessor&);
    ~AttackonPianoAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void setMidiInput(int index);

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

private:
    juce::TextButton mLoadButton{ "Load" };
    std::vector<float> mAudioPoints;
    bool mShouldBePainting{ false };
    AttackonPianoAudioProcessor& audioProcessor;
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttackonPianoAudioProcessorEditor)
};