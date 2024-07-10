/*
  ==============================================================================

    Parameter.h
    Created: 9 Jul 2024 9:56:24pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"


class Parameter2 : juce::Slider::Listener {
public:
    Parameter2(juce::Slider& component);

    inline void setValue(int value) { this->value = value; }
    inline float getValue() { return value; }

    void sliderValueChanged(juce::Slider* slider) override;

    //Parameter(juce::Slider component, int ownerID, int ownerType, int parameterId);
    //Parameter(juce::ComboBox component, int ownerID, int ownerType, int parameterId);
    //void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

    /*inline void setOwnerID(int ownerID) { this->ownerID = ownerID; }
    inline int getOwnerID() { return ownerID; }

    inline void setOwnerType(int ownerType) { this->ownerType = ownerType; }
    inline int getOwnerType() { return ownerType; }

    virtual inline int type() = 0; */

private:
    float value; //Valor guardado

    //int ownerType; //Envelope, LFO, Oscillator...
    //int ownerID; //Envelope 0, LFO 7, Oscillador 3...
    //int parameterTag; // ATTACK, VOLUME, PAN...
};