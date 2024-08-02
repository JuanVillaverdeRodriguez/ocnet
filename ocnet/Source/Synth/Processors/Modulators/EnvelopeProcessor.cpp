/*
  ==============================================================================

    EnvelopeProcessor.cpp
    Created: 12 Jul 2024 4:46:29pm
    Author:  TecNo

  ==============================================================================
*/

#include "EnvelopeProcessor.h"
#include "../../../Utils/Utils.h"

EnvelopeProcessor::EnvelopeProcessor(int id)
{
    setId(id);

    adsrParams.attack = 0.0f;
    adsrParams.decay = 0.0f;
    adsrParams.sustain = 1.0f;
    adsrParams.release = 0.0f;
}

EnvelopeProcessor::~EnvelopeProcessor()
{
}

float EnvelopeProcessor::getNextSample(int sample)
{
    //adsrParams.attack += attackModulationBuffer[sample];
    //adsrParams.decay += attackModulationBuffer[sample];
    //adsrParams.sustain += attackModulationBuffer[sample];
    //adsrParams.release += attackModulationBuffer[sample];

    float currentValue = adsr.getNextSample();
    //DBG("CURRENT ENVELOPE VALUE: " + juce::String(currentValue));
    addToModulationBuffer(currentValue, sample); // Asignar el valor de la modulacion
    return currentValue;
}

void EnvelopeProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    //DBG("EnvelopeProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)");
    adsr.noteOn();
}

void EnvelopeProcessor::stopNote(float velocity, bool allowTailOff)
{
    //DBG("EnvelopeProcessor::stopNote(float velocity, bool allowTailOff)");
    adsr.noteOff();
}

void EnvelopeProcessor::updateParameterValues()
{
    adsrParams.attack = attackParameter->getValue();
    adsrParams.decay = decayParameter->getValue();
    adsrParams.sustain = sustainParameter->getValue();
    adsrParams.release = releaseParameter->getValue();

    attackModulationBuffer = attackParameter->getModulationBuffer(getVoiceNumberId());
    decayModulationBuffer = decayParameter->getModulationBuffer(getVoiceNumberId());
    sustainModulationBuffer = sustainParameter->getModulationBuffer(getVoiceNumberId());
    releaseModulationBuffer = releaseParameter->getModulationBuffer(getVoiceNumberId());

    //DBG(juce::String(adsrParams.attack));
    //DBG(juce::String(adsrParams.decay));
    //DBG(juce::String(adsrParams.sustain));
    //DBG(juce::String(adsrParams.release));

    adsr.setParameters(adsrParams);
}

void EnvelopeProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    adsr.setSampleRate(spec.sampleRate);
    adsr.setParameters(adsrParams);
}

bool EnvelopeProcessor::isActive()
{
    return adsr.isActive();
}

void EnvelopeProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    attackParameter = parameterHandler.syncWithParam(juce::String("Envelopes"), juce::String(getId()), juce::String("attack"));
    decayParameter = parameterHandler.syncWithParam(juce::String("Envelopes"), juce::String(getId()), juce::String("decay"));
    sustainParameter = parameterHandler.syncWithParam(juce::String("Envelopes"), juce::String(getId()), juce::String("sustain"));
    releaseParameter = parameterHandler.syncWithParam(juce::String("Envelopes"), juce::String(getId()), juce::String("release"));
}

/*void EnvelopeProcessor::connectModulationTo(Parameter2& parameter)
{
    parameter.setModulationListener(this);

    modulationListeners.pushback(parameter);
}

void onModulationChanged() {
    for (auto param : modulationListeners) {
        param->updateModulationValue();
    }
}*/