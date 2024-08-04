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

class Subsection : public juce::Component {
public:
    Subsection();

    void inline setId(int id) { this->id = id; }
    int inline getId() { return id; }

    void paint(juce::Graphics& g) override;

    virtual void attachParams(ParameterHandler& parameterHandler) = 0;

    void startDraggingUI();

private:
    int id;

protected:
    juce::TextButton removeButton;

};