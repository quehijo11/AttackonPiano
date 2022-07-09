/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
//This is the constructor for the PluginEditor class... I think - A
AttackonPianoAudioProcessorEditor::AttackonPianoAudioProcessorEditor(AttackonPianoAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    mLoadButton.onClick = [&]() {audioProcessor.loadFile(); };
    addAndMakeVisible(mLoadButton);
    setSize(750, 600);
}

AttackonPianoAudioProcessorEditor::~AttackonPianoAudioProcessorEditor()
{
}

//==============================================================================
void AttackonPianoAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("", getLocalBounds(), juce::Justification::centred, 1);
}

void AttackonPianoAudioProcessorEditor::resized()
{
    mLoadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
}

