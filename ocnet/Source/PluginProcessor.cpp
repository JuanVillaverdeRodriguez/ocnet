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
                       .withOutput ("Output", juce::AudioChannelSet::mono(), true)
                     #endif
                       )
#endif
{
    addSound(new SynthSound());
    for (int i = 0; i < numVoices; ++i)
        addVoice(new SynthVoice(i));

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

    processMidi(midiMessages);
    //applyModulators(); // Aplicar las modulaciones a los parametros (Actualiza el valor de modulacion de cada parametro)
    //updateParameters(); // Actualizar los parametros (Actualiza el valor normal del parametro)
    // Procesar el bloque en cada voz
    renderNextBlock(buffer, midiMessages,  0, buffer.getNumSamples());
}

std::function<void()> getEditor(OcnetAudioProcessor *processor) {
    auto lambda = [processor]()
        {
            jassert(juce::MessageManager::getInstance()->isThisTheMessageThread());

            if (auto activeEditor = dynamic_cast<OcnetAudioProcessorEditor*>(processor->getActiveEditor()))
            {
                activeEditor->addOscillator(0);
            }
        };

    return lambda;

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

void OcnetAudioProcessor::processMidi(juce::MidiBuffer& midiMessages)
{
    // Iterar sobre los mensajes MIDI
    for (const auto metadata : midiMessages)
    {
        const auto message = metadata.getMessage();

        if (message.isNoteOn())
        {
            // Manejar mensaje de Note On
            auto noteNumber = message.getNoteNumber();
            auto velocity = message.getVelocity();
            // Aquí puedes manejar el mensaje de Note On, por ejemplo, activando una voz
            handleNoteOn(noteNumber, velocity);
        }
        else if (message.isNoteOff())
        {
            // Manejar mensaje de Note Off
            auto noteNumber = message.getNoteNumber();
            // Aquí puedes manejar el mensaje de Note Off, por ejemplo, desactivando una voz
            handleNoteOff(noteNumber);
        }
        // Añade más manejadores para otros tipos de mensajes MIDI según sea necesario
    }
}

void OcnetAudioProcessor::handleNoteOn(int noteNumber, float velocity)
{
}

void OcnetAudioProcessor::handleNoteOff(int noteNumber)
{
}

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts) {
    ChainSettings settings;

    settings.volume = apvts.getRawParameterValue("VolumeGain")->load();
    settings.panning = apvts.getRawParameterValue("Panning")->load();


    return settings;
}

juce::AudioProcessorValueTreeState::ParameterLayout OcnetAudioProcessor::createParameterLayout() {
    
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    int maxNumberOfParams = 64;

    //ADSR
    DBG("MAX MODULATORS: " + juce::String(maxNumberOfParams));

    for (int i = 0; i < maxNumberOfParams; i++) {
        layout.add(std::make_unique<juce::AudioParameterFloat>("param_" + juce::String(i), "Attack", juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f), 1.0f));
    }

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OcnetAudioProcessor();
}
