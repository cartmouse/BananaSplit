/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BananaSplitAudioProcessor::BananaSplitAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
    mParameters(*this, nullptr, Identifier("BananaSplit"),
        {
            std::make_unique<AudioParameterFloat>("dial",
            "Dial",
            NormalisableRange<float>(0.0f, 2.0f, 0.01f),
            1.0f)
        })
#endif
{
    mDialParam = mParameters.getRawParameterValue("dial");
}

BananaSplitAudioProcessor::~BananaSplitAudioProcessor()
{
}

//==============================================================================
const String BananaSplitAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BananaSplitAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BananaSplitAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BananaSplitAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BananaSplitAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BananaSplitAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BananaSplitAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BananaSplitAudioProcessor::setCurrentProgram (int index)
{
}

const String BananaSplitAudioProcessor::getProgramName (int index)
{
    return {};
}

void BananaSplitAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void BananaSplitAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto bufferSize = mMaxDelaySeconds * (sampleRate + samplesPerBlock);
    mDelayLine.setSize(getTotalNumInputChannels(), bufferSize);
    mDelayLine.clear();

    mSampleRate = sampleRate;
}

void BananaSplitAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BananaSplitAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void BananaSplitAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    float dialValue = *mDialParam;

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (auto i = 0; i < totalNumInputChannels; i++)
        fillDelayLine(buffer, 0);

    auto delayLineLength = mDelayLine.getNumSamples();
    auto bufferLength = buffer.getNumSamples();
    auto bufferReadPnt0 = buffer.getReadPointer(0);
    auto delayLineReadPnt = mDelayLine.getReadPointer(0);
    float delayTimeMS;

    buffer.addFrom(1, 0, bufferReadPnt0, bufferLength);

    buffer.copyFrom

    mWritePnt += buffer.getNumSamples();
    mWritePnt %= mDelayLine.getNumSamples();
}

//==============================================================================
bool BananaSplitAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BananaSplitAudioProcessor::createEditor()
{
    return new BananaSplitAudioProcessorEditor (*this, mParameters);
}

//==============================================================================
void BananaSplitAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BananaSplitAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void BananaSplitAudioProcessor::fillDelayLine(AudioBuffer<float>& buffer, int channel)
{
    if (mWritePnt + buffer.getNumSamples() <= mDelayLine.getNumSamples())
    {
        mDelayLine.copyFrom(channel, mWritePnt, buffer.getReadPointer(channel), buffer.getNumSamples());
    }
    else
    {
        const auto midPos = mDelayLine.getNumSamples() - mWritePnt;
        mDelayLine.copyFrom(channel, mWritePnt, buffer.getReadPointer(channel), midPos);
        mDelayLine.copyFrom(channel, 0, buffer.getReadPointer(channel, midPos), buffer.getNumSamples() - midPos);
    }
}

// Read from the delay line at a given read point
void BananaSplitAudioProcessor::readDelayLine(AudioBuffer<float>& buffer, int channel, int readPnt)
{
    auto bufferLength = buffer.getNumSamples();
    auto delayLineLength = mDelayLine.getNumSamples();

    if (readPnt + bufferLength <= delayLineLength)
    {
        buffer.copyFrom(channel, 0, mDelayLine.getReadPointer(channel, readPnt), bufferLength);
    }
    else
    {
        const auto midPos = mDelayLine.getNumSamples() - readPnt;
        buffer.copyFrom(channel, 0, mDelayLine.getReadPointer(channel, readPnt), midPos);
        buffer.copyFrom(channel, midPos, mDelayLine.getReadPointer(channel), bufferLength - midPos);
    }
}

// Calculate read point from current write point and a given delay time
int BananaSplitAudioProcessor::calcReadPnt(float timeMS) const
{
    // read delayed signal
    int readPnt = roundToInt(mWritePnt - (mSampleRate * timeMS / 1000.0f));
    if (readPnt < 0)
        readPnt += mDelayLine.getNumSamples();

    return readPnt;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BananaSplitAudioProcessor();
}
