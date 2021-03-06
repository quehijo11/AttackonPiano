/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AttackonPianoAudioProcessor::AttackonPianoAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), mAPValueTreeState(*this, nullptr, "PARAMETERS", mCreateParameters())
#endif
{
    for (int i = 0; i < 8; ++i)
        mSampler.addVoice(new juce::SamplerVoice);
    juce::AudioFormatManager man;
    man.registerBasicFormats();
    // obviously this will fail on someone else's system, so ignore
    std::unique_ptr<juce::AudioFormatReader> reader(man.createReaderFor(juce::File("C:/Users/watso/Desktop/A 4 - Processed_A 4-St.wav")));
    juce::BigInteger notes;
    notes.setRange(0, 127, true);
    mSampler.addSound(new juce::SamplerSound("a", *reader, notes, 60, 0.1, 0.1, 1.0));

    /**************************************************************/
    /*mFormatManager.registerBasicFormats();

    for (int i = 0; i < mNumVoices; i++)
    {
        mSampler.addVoice(new juce::SamplerVoice());
    }*/
}

AttackonPianoAudioProcessor::~AttackonPianoAudioProcessor()
{
    mFormatReader = nullptr;
}

//==============================================================================
const juce::String AttackonPianoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AttackonPianoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AttackonPianoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AttackonPianoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AttackonPianoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AttackonPianoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AttackonPianoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AttackonPianoAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AttackonPianoAudioProcessor::getProgramName (int index)
{
    return {};
}

void AttackonPianoAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AttackonPianoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
    updateADSR();
}

void AttackonPianoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AttackonPianoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AttackonPianoAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //   buffer.clear(channel, 0, buffer.getNumSamples());

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool AttackonPianoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AttackonPianoAudioProcessor::createEditor()
{
    return new AttackonPianoAudioProcessorEditor (*this);
}

//==============================================================================
void AttackonPianoAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AttackonPianoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//void AttackonPianoAudioProcessor::loadFile()
//{
//    mSampler.clearSounds();
//
//    juce::FileChooser chooser{ "Please load a file" }; //eventually this needs to be replaced with launchASync
//
//    if (chooser.browseForFileToOpen()) //Make sure modal loops is permitted to 1 in the pre-processor field in Juce for this to work
//    {
//        auto file = chooser.getResult();
//        mFormatReader = mFormatManager.createReaderFor(file);
//    }
//
//    juce::BigInteger range;
//    range.setRange(0, 128, true);
//
//    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10));
//}
//
//void AttackonPianoAudioProcessor::loadFile(const juce::String& path)
//{
//    mSampler.clearSounds();
//    
//    auto file = juce::File(path);
//    mFormatReader = mFormatManager.createReaderFor(file);
//
//    auto sampleLength = static_cast<int>(mFormatReader->lengthInSamples);
//    mWaveForm.setSize(1, sampleLength);
//    mFormatReader->read(&mWaveForm, 0, sampleLength, 0, true, false);
//
//    auto buffer = mWaveForm.getReadPointer(0);
//
//    for (int sample = 0; sample < mWaveForm.getNumSamples(); ++sample)
//    {
//        DBG(buffer[sample]);
//    }
//
//    juce::BigInteger range;
//    range.setRange(0, 128, true);
//
//    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10));
//}

void AttackonPianoAudioProcessor::updateADSR()
{
    mADSRParameters.attack = mAPValueTreeState.getRawParameterValue("ATTACK")->load();
    mADSRParameters.decay = mAPValueTreeState.getRawParameterValue("DECAY")->load();
    mADSRParameters.sustain = mAPValueTreeState.getRawParameterValue("SUSTAIN")->load();
    mADSRParameters.release = mAPValueTreeState.getRawParameterValue("RELEASE")->load();

    for (int i = 0; i < mSampler.getNumSounds(); ++i)
    {
        if (auto sound = dynamic_cast<juce::SamplerSound*>(mSampler.getSound(i).get()))
        {
            sound->setEnvelopeParameters(mADSRParameters);
        }
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout AttackonPianoAudioProcessor::mCreateParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.0f, 5.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.0f, 3.0f, 2.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.0f, 5.0f, 2.0f));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AttackonPianoAudioProcessor();
}
