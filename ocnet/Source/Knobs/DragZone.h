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

using namespace Ocnet;

class DragZone : public juce::Component, public juce::DragAndDropContainer
{
public:
    DragZone();

    void setParentContainerAndComponent(juce::DragAndDropContainer& parentContainer, Subsection& parentComponent);

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event);
    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;
    void resized() override;
    void paint(juce::Graphics& g) override;
private:
    bool mouseHovering;
    juce::DragAndDropContainer* parentContainer;
    Subsection* parentComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DragZone)


};