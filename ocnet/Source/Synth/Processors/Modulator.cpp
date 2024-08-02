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

void Modulator::updateModulationValue() {
    for (auto& parameter : parametersModulating) {
        parameter->updateModulationValue(modulationBuffer, getVoiceNumberId());
    }
    modulationBuffer.clearQuick();
}

void Modulator::connectModulation(std::shared_ptr<Parameter2> parameterToModulate) {
    parametersModulating.push_back(parameterToModulate);
    //DBG("ConnectModulation(Parameter2* parameterToModulate)");
}

void Modulator::addToModulationBuffer(float newModulationValue, int sample) {
    modulationBuffer.add(newModulationValue);
}