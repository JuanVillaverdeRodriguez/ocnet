/*
  ==============================================================================

    DragZone.h
    Created: 20 Jul 2024 7:23:19pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../../Utils/OcnetTypes.h"
#include "../../../Controller/GUI_EventHandler.h"

using namespace Ocnet;

class ModulatorsSubsection;

class DragZone : public juce::Component, public juce::DragAndDropContainer
{
public:
    DragZone(GUI_EventHandler& eventHandler);

    void setParentContainerAndComponent(juce::DragAndDropContainer& parentContainer, ModulatorsSubsection& parentComponent);

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event);
    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;
    void resized() override;
    void paint(juce::Graphics& g) override;
private:
    GUI_EventHandler& eventHandler;

    bool mouseHovering;
    juce::DragAndDropContainer* parentContainer;
    ModulatorsSubsection* parentComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DragZone)
};