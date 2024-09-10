/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <windows.h>

//==============================================================================

OcnetAudioProcessor::OcnetAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), paramsSynced(false)
#endif
{

    wavetableOscillatorTest.setProcessorInfo(processorInfo);

    setNoteStealingEnabled(true);

    addSound(new SynthSound());
    for (int i = 0; i < numVoices; ++i)
        addVoice(new SynthVoice(i, parameterHandler, processorInfo));

}



OcnetAudioProcessor::~OcnetAudioProcessor()
{
}

//==============================================================================
const juce::String OcnetAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OcnetAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OcnetAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OcnetAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OcnetAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OcnetAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OcnetAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OcnetAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OcnetAudioProcessor::getProgramName (int index)
{
    return {};
}

void OcnetAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void OcnetAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    processorInfo.prepare(sampleRate, samplesPerBlock);

    setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
            voice->setParameterHandler(parameterHandler);
        }
    }
}

void OcnetAudioProcessor::releaseResources()
{
    DBG("OcnetAudioProcessor::releaseResources()");
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->releaseResources();
        }
    }

    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OcnetAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void OcnetAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Limpiar el buffer
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    if (paramsSynced)  
        updateSynthParameters();

    processorInfo.process(getPlayHead(), buffer.getNumSamples());

    // Procesar el bloque en cada voz
    renderNextBlock(buffer, midiMessages,  0, buffer.getNumSamples());

}

//==============================================================================
bool OcnetAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OcnetAudioProcessor::createEditor()
{
    return new OcnetAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void OcnetAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto xml = parameterHandler.getRootTree().createXml();
    copyXmlToBinary(*xml, destData);
}

void OcnetAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameterHandler.getRootTree().getType()))
            parameterHandler.getRootTree().fromXml(*xmlState);
}

juce::AudioProcessorValueTreeState::ParameterLayout OcnetAudioProcessor::createParameterLayout() {
    
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    int maxNumberOfMacros = 12;

    for (int i = 1; i < maxNumberOfMacros+1; i++) {
        layout.add(std::make_unique<juce::AudioParameterFloat>("Macro_" + juce::String(i) + "_macro", "Value", juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f), 1.0f));
    }

    return layout;
}

void OcnetAudioProcessor::updateSynthParameters()
{
    processorInfo.legatoInfo.glideValue = glideParameter->getValue() == 0.0f ? 0.01 : glideParameter->getValue();
    //glideModulationBuffer = glideParameter->getModulationBuffer(0);

    numVoices = numVoicesParameter->getValue();
    //numVoicesModulationBuffer = numVoicesParameter->getModulationBuffer(0);

    processorInfo.legatoInfo.legatoIsActive = legatoParameter->getState();

    setThisNumberOfVoices(numVoices);

    /*if (numVoices != getNumberOfActiveVoices()) {
        int voicesToDisable = 8 - numVoices;
        disableThisNumberOfVoices(voicesToDisable);
    }*/
}

void OcnetAudioProcessor::syncSynthParameters()
{
    paramsSynced = true;
    numVoicesParameter = parameterHandler.syncWithSliderParam("Synth_-1_numVoices");
    glideParameter = parameterHandler.syncWithSliderParam("Synth_-1_glide");
    legatoParameter = parameterHandler.syncWithButtonParam("Synth_-1_legato");
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OcnetAudioProcessor();
}
