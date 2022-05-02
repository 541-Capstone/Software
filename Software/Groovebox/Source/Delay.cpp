/*
  ==============================================================================

    Delay.cpp
    Created: 27 Apr 2022 11:14:52pm
    Author:  Dima

  ==============================================================================
*/

#include "Delay.h"

Delay::Delay(te::PluginCreationInfo info) : te::Plugin(info)
{
	sampleRate = info.edit.engine.getDeviceManager().getSampleRate();
}

Delay::~Delay()
{
    notifyListenersOfDeletion();
}

const char* Delay::getPluginName() { return NEEDS_TRANS("GrooveboxDelay"); }

juce::String Delay::getName() { return getPluginName(); }
juce::String Delay::getPluginType() { return xmlTypeName; }
bool Delay::needsConstantBufferSize() { return false; }
juce::String Delay::getSelectableDescription() { return getName(); }

void Delay::initialise(const te::PluginInitialisationInfo& info)
{
    auto delayBufferSize = info.sampleRate * 2.0; //2 seconds of audio
    delayBuffer.setSize(2, (int)delayBufferSize);
}

void Delay::deinitialise()
{
}

void Delay::applyToBuffer(const te::PluginRenderContext& fc)
{

	juce::ScopedNoDenormals noDenormals;

	auto bufferSize = fc.destBuffer->getNumSamples();
	auto delayBufferSize = delayBuffer.getNumSamples();

	te::clearChannels(*fc.destBuffer, 2, -1, fc.bufferStartSample, fc.bufferNumSamples);

	for (int channel = 0; channel < fc.destBuffer->getNumChannels(); ++channel)
	{
		auto writeBuffer = fc.destBuffer->getWritePointer(channel);
		fillBuffer(channel, bufferSize, delayBufferSize, writeBuffer);
		readFromBuffer(*fc.destBuffer, delayBuffer, channel, bufferSize, delayBufferSize);
	}

	writePosition += bufferSize; //keeps track of where to copy to in the delay buffer
	writePosition %= delayBufferSize; //ensures this always stays between 0 and delayBufferSize
}

//fills the delay buffer
void Delay::fillBuffer(int channel, int bufferSize, int delayBufferSize, float* channelData)
{
	//Check if main buffer copies to delay buffer w/o needing to wrap...
	if (delayBufferSize > bufferSize + writePosition)
	{
		//copy main buffer contents to delay buffer
		//copyFrom is a function in the AudioBuffer class
		delayBuffer.copyFrom(channel, writePosition, channelData, bufferSize);
	}
	else
	{
		//Determine how much space is left at the end of the delay buffer
		auto numSamplesToEnd = delayBufferSize - writePosition;

		//copy that amount of contents to the end...
		delayBuffer.copyFrom(channel, writePosition, channelData, numSamplesToEnd);

		//calculate how much contents is remaining to copy
		auto numSamplesAtStart = bufferSize - numSamplesToEnd;

		//copy remaining amount to beginning of delay buffer
		delayBuffer.copyFrom(channel, 0, channelData + numSamplesToEnd, numSamplesAtStart);
	}
}

//reads from the delay buffer
void Delay::readFromBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, int channel, int bufferSize, int delayBufferSize)
{
	//1 second in the past
	int readPosition = writePosition - sampleRate;

	//makes sure readPosition stays positive
	if (readPosition < 0)
	{
		readPosition += delayBufferSize;
	}

	//Check if delay buffer adds to main buffer w/o needing to wrap...
	if (readPosition + bufferSize < delayBufferSize)
	{
		//add delay buffer contents to main buffer
		//addFromWithRamp is a function in the AudioBuffer class
		buffer.addFrom(channel, 0, delayBuffer.getReadPointer(channel, readPosition), bufferSize);
	}
	else
	{
		//Determine how much space is left at the end of the delay buffer
		auto numSamplesToEnd = delayBufferSize - readPosition;

		//add that amount of contents to the beginning of main buffer
		buffer.addFrom(channel, 0, delayBuffer.getReadPointer(channel, readPosition), numSamplesToEnd);

		//calculate how much contents is remaining to add
		auto numSamplesAtStart = bufferSize - numSamplesToEnd;

		//add remaining amount to end of main buffer
		buffer.addFrom(channel, numSamplesToEnd, delayBuffer.getReadPointer(channel, 0), numSamplesAtStart);
	}
}

void Delay::contextControl(const juce::MidiMessageMetadata &metadata){
    
}

IPlugin::Parameter Delay::getParameterValue(int index){
    return {};
}
