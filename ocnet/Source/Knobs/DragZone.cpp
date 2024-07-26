/*
  ==============================================================================

    DragZone.cpp
    Created: 20 Jul 2024 7:23:19pm
    Author:  TecNo

  ==============================================================================
*/

#include "DragZone.h"

DragZone::DragZone()
{
    setSize(50, 50);       
}

void DragZone::setParentContainerAndComponent(juce::DragAndDropContainer& parentContainer2, Subsection& parentComponent2)
{
    parentContainer = &parentContainer2;
    parentComponent = &parentComponent2;
}

void DragZone::mouseDown(const juce::MouseEvent& event)
{
    DBG("DragZone::mouseDown(const juce::MouseEvent& event)");
    if (parentContainer != nullptr)
    {
        DBG("parentContainer != nullptr");

        juce::var dragDescription = "LFO1"; // Descripción de lo que se está arrastrando
        parentContainer->startDragging(dragDescription, parentComponent);
        //parentComponent->startDraggingUI();
    }
}

void DragZone::resized()
{
}

void DragZone::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkcyan);
    g.setColour(juce::Colours::darkgrey);
    g.drawRect(getLocalBounds(), 2);
}
