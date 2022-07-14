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
    : AudioProcessorEditor(&p), audioProcessor(p), keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    mLoadButton.onClick = [&]() {audioProcessor.loadFile(); };
    addAndMakeVisible(mLoadButton);
    addAndMakeVisible(keyboardComponent);
    setSize(800, 600);
}

AttackonPianoAudioProcessorEditor::~AttackonPianoAudioProcessorEditor()
{
}

//==============================================================================
void AttackonPianoAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    /*g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("", getLocalBounds(), juce::Justification::centred, 1);*/

    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);

    juce::Path p;
    p.clear();

    if (mShouldBePainting)
    {
        juce::Path p;
        mAudioPoints.clear();

        auto waveform = audioProcessor.getWaveForm();
        auto ratio = waveform.getNumSamples() / getWidth();
        auto buffer = waveform.getReadPointer(0);

        //scale audio file to window on x axis
        for (int sample = 0; sample < waveform.getNumSamples(); sample += ratio)
        {
            mAudioPoints.push_back(buffer[sample]);
        }

        p.startNewSubPath(0, getHeight() / 2);

        //scale on y axis
        for (int sample = 0; sample < mAudioPoints.size(); ++sample)
        {
            auto point = juce::jmap<float>(mAudioPoints[sample], -1.0f, 1.0f, 200, 0);
            p.lineTo(sample, point);
        }

        g.strokePath(p, juce::PathStrokeType(2)); //determines thickness of the waveform
        mShouldBePainting = false;
    }

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);

    /*
    if (audioProcessor.getNumSamplerSounds() > 0)
    {
        g.fillAll(juce::Colours::red);
        g.drawText("Sample Loaded", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, juce::Justification::centred);
    }
    else
    {
        g.drawText("Load a Sample", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, juce::Justification::centred);
    }*/
}

void AttackonPianoAudioProcessorEditor::resized()
{
    //mLoadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
    keyboardComponent.setBounds(0, getHeight() - 175, 750, 175);
}

bool AttackonPianoAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif"))
        {
            return true;
        }
    }
    
    return false;
}

void AttackonPianoAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(files))
        {
            mShouldBePainting = true;
            audioProcessor.loadFile(file);
        }
    }

    repaint();
}