/*
  ==============================================================================

    DragZone.h
    Created: 20 Jul 2024 7:23:19pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../GUI_Subsections/Subsection.h"
class DragZone : public juce::Component, public juce::DragAndDropContainer
{
public:
    DragZone();

    void setParentContainerAndComponent(juce::DragAndDropContainer& parentContainer, Subsection& parentComponent);

    void mouseDown(const juce::MouseEvent& event) override;
    void resized() override;
    void paint(juce::Graphics& g) override;
private:
    juce::DragAndDropContainer* parentContainer;
    Subsection* parentComponent;

};