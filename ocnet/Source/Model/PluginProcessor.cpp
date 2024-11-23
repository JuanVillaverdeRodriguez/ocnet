/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "../Controller/PluginEditor.h"
#include <windows.h>
#include "Synth/Processors/Effects/FilterProcessor.h"
#include "Synth/Processors/Effects/EqualizerProcessor.h"
#include "Synth/Processors/Effects/DelayProcessor.h"

//==============================================================================
std::mutex 

; // Declarar el mutex

OcnetAudioProcessor::OcnetAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), paramsSynced(false), allowProcessBlock(true)
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
    procesSpec.maximumBlockSize = samplesPerBlock;
    procesSpec.numChannels = 2;
    procesSpec.sampleRate = sampleRate;

    processorInfo.hostInfo.prepare(sampleRate, samplesPerBlock);

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

void OcnetAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Limpia las salidas no utilizadas.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Verifica si hay operaciones pendientes (añadir o eliminar).
    if (pendingAddEffect.load())
    {
        std::lock_guard<std::mutex> lock(effectsMutex);

        // Añadir efecto si `effectToAdd` no es nulo.
        if (effectToAdd) {
            effectsProcessorsList.push_back(std::move(effectToAdd));
        }

        // Eliminar efectos marcados en `effectsToDelete`.
        for (int id : effectsToDelete) {
            effectsProcessorsList.erase(
                std::remove_if(effectsProcessorsList.begin(), effectsProcessorsList.end(),
                    [id](const std::unique_ptr<Effector>& effector) {
                        return effector->getId() == id;
                    }),
                effectsProcessorsList.end()
            );
        }
        effectsToDelete.clear(); // Limpia los IDs después de eliminarlos.
        pendingAddEffect.store(false); // Resetear el flag.
    }

    if (paramsSynced)
        updateSynthParameters();

    processorInfo.hostInfo.process(getPlayHead(), buffer.getNumSamples());
    renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    updateEffectsParameters();

    if (isAnyVoiceActive() || !longLastingEffectsAreOver()) {
        processEffects(buffer);
    }
}

bool OcnetAudioProcessor::isBufferSilent(const juce::AudioBuffer<float>& buffer) const
{
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        auto minMax = juce::FloatVectorOperations::findMinAndMax(buffer.getReadPointer(channel), buffer.getNumSamples());
        if (minMax.getStart() != 0.0f || minMax.getEnd() != 0.0f) {
            return false; // El buffer no está en silencio.
        }
    }
    return true; // Todos los valores son cero.
}

void OcnetAudioProcessor::deleteEffect(int processorID)
{
    {
        std::lock_guard<std::mutex> lock(effectsMutex);
        effectsToDelete.push_back(processorID); // Marca el ID para eliminación.
        pendingAddEffect.store(true);           // Indica que hay operaciones pendientes.
    }
}

void OcnetAudioProcessor::moveEffect(int id, int positions)
{
    // Encontrar la posicion en la lista
    int initIndex = Utils::findElementPositionByID(effectsProcessorsList, id);

    // Mueve el elemento
    Utils::moveElement(effectsProcessorsList, initIndex, positions);
}

void OcnetAudioProcessor::bypassEffect(int id, bool bypassed)
{
    std::unique_ptr<Effector>* effector = nullptr;

    effector = Utils::findElementByID(effectsProcessorsList, id);
    if (effector) {
        effector->get()->setBypassed(bypassed);
        return;
    }
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

bool OcnetAudioProcessor::longLastingEffectsAreOver()
{
    for (auto& effect : effectsProcessorsList) {
        if (!effect->isBypassed()) {
            if (effect->isLongLasting()) 
                return false;
        }
    }

    return true;
}

void OcnetAudioProcessor::processEffects(juce::AudioBuffer<float>& buffer)
{
    for (auto& processor : effectsProcessorsList) {
        if (!processor->isBypassed()) {
            processor->processBlock(buffer);
        }
    }
}

void OcnetAudioProcessor::updateEffectsParameters()
{
    for (auto& processor : effectsProcessorsList) {
        processor->updateParameterValues();
    }
}

void OcnetAudioProcessor::addEffect(int processorType, int id)
{
    std::unique_ptr<Effector> newEffect;

    switch (processorType)
    {
    case Filter:
        newEffect = std::make_unique<FilterProcessor>(id);
        break;

    case Distortion:
        newEffect = std::make_unique<DistortionProcessor>(id);
        break;

    case Reverb:
        newEffect = std::make_unique<ReverbProcessor>(id);
        break;

    case Equalizer:
        newEffect = std::make_unique<EqualizerProcessor>(id);
        break;

    case Delay:
        newEffect = std::make_unique<DelayProcessor>(id);
        break;

    default:
        return;
    }

    // Configurar el efecto antes de añadirlo.
    newEffect->setVoiceNumberId(8);
    newEffect->syncParams(parameterHandler);
    newEffect->prepareToPlay(procesSpec);

    // Almacena el efecto y marca la operación como pendiente.
    {
        std::lock_guard<std::mutex> lock(effectsMutex);
        effectToAdd = std::move(newEffect);
        pendingAddEffect.store(true); // Marca que hay un efecto pendiente.
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OcnetAudioProcessor();
}
