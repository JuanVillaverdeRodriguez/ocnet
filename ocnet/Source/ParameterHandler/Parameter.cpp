/*
  ==============================================================================

    Parameter.cpp
    Created: 9 Jul 2024 9:56:24pm
    Author:  TecNo

  ==============================================================================
*/

#include "Parameter.h"

Parameter2::Parameter2(juce::Slider& component) {
    value = 0;

    component.addListener(this);
}


void Parameter2::sliderValueChanged(juce::Slider* slider) {
    value = slider->getValue();
}

/*Parameter::Parameter(juce::ComboBox component, int ownerID, int ownerType, int parameterTag) {
    value = 0;
    component.addListener(this);

    /this->ownerID = ownerID;
    this->ownerType = ownerType;
    this->parameterTag = parameterTag;
}

Parameter::Parameter(juce::Slider component, int ownerID, int ownerType, int parameterTag) {
    value = 0;
    component.addListener(this);

    this->ownerID = ownerID;
    this->ownerType = ownerType;
    this->parameterTag = parameterTag;

}

void Parameter::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{

}*/
