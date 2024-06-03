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
                       )
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++) {

        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }

    
}

void OcnetAudioProcessor::releaseResources()
{
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

   


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());


    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) { // Si la voz es del tipo juce::SynthesiserVoice...
            
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");

            voice->updateADSR(attack.load(), decay.load(), sustain.load(), release.load());
            // OSC controls
            // ADSR
            // LFO
            
        }
    }

    synth.renderNextBlock(buffer, midiMessages,  0, buffer.getNumSamples());

    /*DBG("INPUT CHANNELS: " + std::to_string((buffer.getNumSamples())));



    auto chainSettings = getChainSettings(apvts);
    noteOnVel = chainSettings.volume;

    const auto panAngle = chainSettings.volume * juce::MathConstants<float>::halfPi;
    const auto* const* read = buffer.getArrayOfReadPointers();

    auto* const* write = buffer.getArrayOfWritePointers();

    for (auto channel = 0; channel < buffer.getNumChannels(); ++channel) {

        const auto pan = channel == 0 ? std::cos(panAngle) : std::sin(panAngle);

        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) {

            write[channel][sample] = read[channel][sample] * pan;
        }
    }    */
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
}

void OcnetAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts) {
    ChainSettings settings;

    settings.volume = apvts.getRawParameterValue("VolumeGain")->load();
    settings.panning = apvts.getRawParameterValue("Panning")->load();

    return settings;
}
juce::AudioProcessorValueTreeState::ParameterLayout OcnetAudioProcessor::createParameterLayout() {

    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    //std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;


    //VOLUME
    layout.add(std::make_unique<juce::AudioParameterFloat> ("VolumeGain", "VolumeGain", juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f), 0.5f));

    //PANNING
    layout.add(std::make_unique<juce::AudioParameterFloat>("Panning", "Panning", juce::NormalisableRange<float>(-1.f, 1.f, 0.01f, 1.f), 0.0f));
    
    
    //OSCILLATOR
    layout.add(std::make_unique<juce::AudioParameterChoice>("OSC", "Oscillator", juce::StringArray {"Sine", "Saw", "Square"}, 0));

    //ADSR
    layout.add(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f), 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f), 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f), 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f), 0.0f));

    return layout;
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OcnetAudioProcessor();
}
