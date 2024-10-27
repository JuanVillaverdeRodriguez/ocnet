/*
  ==============================================================================

    SliderParameter.h
    Created: 6 Aug 2024 8:36:18pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../../Utils/Utils.h"

class SliderParameter : public juce::ValueTree::Listener {
public:
    SliderParameter(const juce::String& parameterID);
    SliderParameter(const juce::String& parameterID, float value);
    ~SliderParameter() = default;

    juce::String getParameterID();

    void setValue(float value);
    float getValue();

    juce::Array<float> getModulationBuffer(int voice);
    void setModulationBuffer(juce::Array<float> newModulationBuffer, int voice);

    void addTreeListener(juce::ValueTree tree);
    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;

    float getModulatedValue(int voice, float min = 0.0f, float max = 1.0f);
private:
    juce::String parameterID;
    int arraySize;
    float value;

    juce::Identifier parameterIdentifier;
    juce::ValueTree tree;

    juce::Array<float> modulations[9]; // 8 voces + 1 para los efectos...

};