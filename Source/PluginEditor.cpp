/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AttackonPianoAudioProcessorEditor::AttackonPianoAudioProcessorEditor(AttackonPianoAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    mLoadButton.onClick = [&]() {audioProcessor.loadFile(); };
    addAndMakeVisible(mLoadButton); // displays the Load button. Button is currently covered up
    addAndMakeVisible(keyboardComponent); //displays the midi keyboard
    
    /*********************************************************************************************
    ADSR Sliders
    **********************************************************************************************/
    //attack sliders
    mAttackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mAttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mAttackSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    mAttackSlider.setRange(0.0f, 5.0f, 0.01f);
    addAndMakeVisible(mAttackSlider); //displays the attack slider
    //attack slider label 
    mAttackLabel.setFont(10.0f); 
    mAttackLabel.setText("Attack", juce::NotificationType::dontSendNotification);
    mAttackLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::green);
    mAttackLabel.setJustificationType(juce::Justification::centredTop);
    mAttackLabel.attachToComponent(&mAttackSlider, false);

    mAttackAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "ATTACK", mAttackSlider);

    //decay sliders
    mDecaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mDecaySlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    mDecaySlider.setRange(0.0f, 5.0f, 0.01f);
    addAndMakeVisible(mDecaySlider); //displays the decay slider
    //decay slider label 
    mDecayLabel.setFont(10.0f);
    mDecayLabel.setText("Decay", juce::NotificationType::dontSendNotification);
    mDecayLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::green);
    mDecayLabel.setJustificationType(juce::Justification::centredTop);
    mDecayLabel.attachToComponent(&mDecaySlider, false);

    mDecayAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DECAY", mDecaySlider);

    //sustain sliders
    mSustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mSustainSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    mSustainSlider.setRange(0.0f, 1.0f, 0.01f);
    addAndMakeVisible(mSustainSlider); //displays the sustain slider
    //sustain slider label 
    mSustainLabel.setFont(10.0f);
    mSustainLabel.setText("Sustain", juce::NotificationType::dontSendNotification);
    mSustainLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::green);
    mSustainLabel.setJustificationType(juce::Justification::centredTop);
    mSustainLabel.attachToComponent(&mSustainSlider, false);

    mSustainAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "SUSTAIN", mSustainSlider);

    //release sliders
    mReleaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mReleaseSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    mReleaseSlider.setRange(0.0f, 1.0f, 0.01f);
    addAndMakeVisible(mReleaseSlider); //displays the release slider
    //release slider label 
    mReleaseLabel.setFont(10.0f);
    mReleaseLabel.setText("Release", juce::NotificationType::dontSendNotification);
    mReleaseLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::green);
    mReleaseLabel.setJustificationType(juce::Justification::centredTop);
    mReleaseLabel.attachToComponent(&mReleaseSlider, false);

    mReleaseAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "RELEASE", mReleaseSlider);
    /*   End of sliders   ***************************************************************************/

    setSize(800, 600); //sets the size of the entire window
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

    const auto sliderStartX = 0.6f;
    const auto sliderStartY = 0.6f;
    const auto sliderWidth = 0.1f;
    const auto sliderHeight = 0.4f;
    mAttackSlider.setBoundsRelative(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    mDecaySlider.setBoundsRelative(sliderStartX + sliderWidth, sliderStartY, sliderWidth, sliderHeight);
    mSustainSlider.setBoundsRelative(sliderStartX + (sliderWidth * 2), sliderStartY, sliderWidth, sliderHeight);
    mReleaseSlider.setBoundsRelative(sliderStartX + (sliderWidth * 3), sliderStartY, sliderWidth, sliderHeight);
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
