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
    DBG("Parameter2::sliderValueChanged(juce::Slider* slider)");

    value = slider->getValue();
    valueTree.setProperty(this->propertyIdentifier, slider->getValue(), nullptr);
}

void Parameter2::setTreeListener(juce::ValueTree tree, juce::String & propertyName)
{
    DBG("Parameter2::setTreeListener(juce::ValueTree tree, juce::Identifier & propertyIdentifier)");
    valueTree = tree;
    juce::Identifier propertyNameIdentifier(propertyName); // Attack, decay, volume...

    this->propertyIdentifier = propertyNameIdentifier;
    valueTree.addListener(this);
}

void Parameter2::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    DBG("Parameter2::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)");
    DBG("Parameter2::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)");
    DBG("PROPERTY" + juce::String(property.getCharPointer())  + "CHANGED: " + juce::String(treeWhosePropertyHasChanged.getProperty(property).toString()));

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
