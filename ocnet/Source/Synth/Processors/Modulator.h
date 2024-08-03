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
    //~Modulator() = default;
    virtual ~Modulator() = default;

    void updateModulationValue();

    void connectModulation(std::shared_ptr<Parameter2> parameterToModulate);

    void addToModulationBuffer(float newModulationValue, int sample);

    //float** getModulationBuffer() { return modulationBuffer; }

    
    //disconectParameter
private:
    std::vector<std::shared_ptr<Parameter2>> parametersModulating;

    //float modulationBuffer[8][8192];

    juce::Array<float> modulationBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Modulator)

};