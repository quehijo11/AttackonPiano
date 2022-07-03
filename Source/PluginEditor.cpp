/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
//This is the constructor for the PluginEditor class... I think - A
AttackonPianoAudioProcessorEditor::AttackonPianoAudioProcessorEditor (AttackonPianoAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), 
      keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    addAndMakeVisible(keyboardComponent);
    setSize(750, 600);

    addAndMakeVisible(midiInputListLabel);
    midiInputListLabel.setText("Midi Input", juce::dontSendNotification);
    midiInputListLabel.attachToComponent(&midiInputList, true);

    auto midiInputs = juce::MidiInput::getAvailableDevices();
    addAndMakeVisible(midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");

    juce::StringArray midiInputNames;
    for (auto input : midiInputs)
        midiInputNames.add(input.name);

    midiInputList.addItemList(midiInputNames, 1);
    midiInputList.onChange = [this] {setMidiInput(midiInputList.getSelectedItemIndex());  };
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
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void AttackonPianoAudioProcessorEditor::setMidiInput(int index) //Incomplete. Come back to this.
{
    auto list = juce::MidiInput::getAvailableDevices();

    //deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier, )
}
