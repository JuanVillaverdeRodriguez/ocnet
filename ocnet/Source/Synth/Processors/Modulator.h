/*
  ==============================================================================

    Modulator.h
    Created: 23 Jul 2024 2:51:05pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "Processor.h"

class Modulator : public Processor {
public:
    Modulator();

    void inline updateModulationValue() { 
        for (auto parameter : parametersModulating) {
            parameter->updateModulationValue(modulationValue);
        }
    }

    void inline connectModulation(Parameter2* parameterToModulate) { 
        parametersModulating.push_back(parameterToModulate); 
        DBG("ConnectModulation(Parameter2* parameterToModulate)");
    }

    void inline setModulationValue(float newModulationValue) { modulationValue = newModulationValue; /*DBG("CURRENT ENVELOPE VALUE: " + juce::String(modulationValue));*/ }
    float inline getModulationValue() { return modulationValue; }

    //disconectParameter
private:
    std::vector<Parameter2*> parametersModulating;
    float modulationValue;
};