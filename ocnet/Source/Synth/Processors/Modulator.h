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
    virtual ~Modulator() = default;

    void inline updateModulationValue() { 
        for (auto parameter : parametersModulating) {
            parameter->updateModulationValue(modulationBuffer, getVoiceNumberId());
        }
        modulationBuffer.clearQuick();
    }

    void inline connectModulation(Parameter2* parameterToModulate) { 
        parametersModulating.push_back(parameterToModulate); 
        DBG("ConnectModulation(Parameter2* parameterToModulate)");
    }

    void inline addToModulationBuffer(float newModulationValue, int sample) { modulationBuffer.add(newModulationValue); /*DBG("CURRENT ENVELOPE VALUE: " + juce::String(modulationValue));*/ }

    //float** getModulationBuffer() { return modulationBuffer; }

    
    //disconectParameter
private:

    std::vector<Parameter2*> parametersModulating;
    //float modulationBuffer[8][8192];

    juce::Array<float> modulationBuffer;
};