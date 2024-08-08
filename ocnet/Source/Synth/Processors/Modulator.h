/*
  ==============================================================================

    Modulator.h
    Created: 23 Jul 2024 2:51:05pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "Processor.h"
#include "../../ParameterHandler/SliderParameter.h"
#include "../../ParameterHandler/Modulation.h"
class Modulator : public Processor {
public:
    Modulator();
    //~Modulator() = default;
    virtual ~Modulator() = default;

    void updateModulationValue();

    void connectModulation(const ParameterHandler& parameterHandler, std::shared_ptr<SliderParameter> parameterToModulate, const juce::String& parameterID);

    void addToModulationBuffer(float newModulationValue, int sample);

    //float** getModulationBuffer() { return modulationBuffer; }

    
    //disconectParameter
private:
    std::vector<std::shared_ptr<Modulation>> modulationsVector;

    juce::Array<float> modulationBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Modulator)

};