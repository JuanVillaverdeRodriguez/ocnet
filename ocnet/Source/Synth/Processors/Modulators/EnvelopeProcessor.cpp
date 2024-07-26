/*
  ==============================================================================

    EnvelopeProcessor.cpp
    Created: 12 Jul 2024 4:46:29pm
    Author:  TecNo

  ==============================================================================
*/

#include "EnvelopeProcessor.h"

EnvelopeProcessor::EnvelopeProcessor(int id)
{
    setId(id);

    adsrParams.attack = 0.0f;
    adsrParams.decay = 0.0f;
    adsrParams.sustain = 1.0f;
    adsrParams.release = 0.0f;
}

float EnvelopeProcessor::getNextSample()
{
    float currentValue = adsr.getNextSample();
    //DBG("CURRENT ENVELOPE VALUE: " + juce::String(currentValue));
    setModulationValue(currentValue); // Asignar el valor de la modulacion
    updateModulationValue(); // Actualizar la modulacion en los parametros asignados
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

void EnvelopeProcessor::updateParameterValues(ParameterHandler parameterHandler)
{
    adsrParams.attack = parameterHandler.getParameterValue(juce::String("Envelopes"), juce::String(getId()), juce::String("attack"));
    adsrParams.decay = parameterHandler.getParameterValue(juce::String("Envelopes"), juce::String(getId()), juce::String("decay"));
    adsrParams.sustain = parameterHandler.getParameterValue(juce::String("Envelopes"), juce::String(getId()), juce::String("sustain"));
    adsrParams.release = parameterHandler.getParameterValue(juce::String("Envelopes"), juce::String(getId()), juce::String("release"));

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