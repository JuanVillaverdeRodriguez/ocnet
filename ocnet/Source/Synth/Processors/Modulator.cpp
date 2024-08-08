/*
  ==============================================================================

    Modulator.cpp
    Created: 23 Jul 2024 2:51:05pm
    Author:  TecNo

  ==============================================================================
*/

#include "Modulator.h"

Modulator::Modulator()
{
}

// Update modulations
void Modulator::updateModulationValue() {
    for (auto& modulation : modulationsVector) {
        modulation->updateModulationBuffer(modulationBuffer, getVoiceNumberId());
    }
    modulationBuffer.clearQuick();
}

void Modulator::connectModulation(const ParameterHandler& parameterHandler, std::shared_ptr<SliderParameter> parameterToModulate, const juce::String& parameterID) {
    DBG("Modulator::connectModulation(const ParameterHandler& parameterHandler, std::shared_ptr<SliderParameter> parameterToModulate, const juce::String& parameterID)");
    DBG(parameterID);

    modulationsVector.push_back(std::make_shared<Modulation>(parameterToModulate));
    modulationsVector.back()->syncParams(parameterHandler, parameterID);
}

void Modulator::addToModulationBuffer(float newModulationValue, int sample) {
    modulationBuffer.add(newModulationValue);
}