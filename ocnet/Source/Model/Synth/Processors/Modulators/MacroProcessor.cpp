/*
  ==============================================================================

    MacroProcessor.cpp
    Created: 31 Aug 2024 7:25:05pm
    Author:  TecNo

  ==============================================================================
*/

#include "MacroProcessor.h"

MacroProcessor::MacroProcessor(int id, int macroID) : selectedMacro(macroID)
{
    setId(id);
}

MacroProcessor::~MacroProcessor()
{
}

float MacroProcessor::getNextSample(int sample)
{
    float nextValue = macroValue.getNextValue();
    addToModulationBuffer(nextValue, sample); // Asignar el valor de la modulacion
    return nextValue;
}

void MacroProcessor::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
}

void MacroProcessor::stopNote(float velocity, bool allowTailOff)
{
}

void MacroProcessor::updateParameterValues()
{
    macroValue.setTargetValue(*parameterHandler->apvts.getRawParameterValue(juce::String("Macro_") + juce::String(selectedMacro) + juce::String("_macro")));
}

void MacroProcessor::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    macroValue.reset(spec.sampleRate, 0.0005);
}

void MacroProcessor::syncParams(const ParameterHandler& parameterHandler)
{
    this->parameterHandler = &parameterHandler;
}

void MacroProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{
}
