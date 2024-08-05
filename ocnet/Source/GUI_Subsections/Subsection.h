/*
  ==============================================================================

    Subsection.h
    Created: 8 Jul 2024 6:35:23pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../ParameterHandler/ParameterHandler.h"
#include "../GUI_EventHandler.h"

class Subsection : public juce::Component, juce::Button::Listener {
public:
    Subsection(GUI_EventHandler& eventHandler);
    virtual ~Subsection() = default;

    void inline setId(int id) { this->id = id; }
    int inline getId() const { return id; }

    void paint(juce::Graphics& g) override;

    void buttonClicked(juce::Button* clickedButton) override;


    virtual void attachParams(ParameterHandler& parameterHandler) {};

    void sectionResized();

    virtual juce::String getType() { return juce::String(""); };

private:
    int id;
    GUI_EventHandler& eventHandler;


protected:
    juce::TextButton removeButton;
    juce::TextButton moveUpButton;
    juce::TextButton moveDownButton;

    juce::DrawableText subsectionName;

    int defaultKnobSize;


};