/*
  ==============================================================================

    Distortion.h
    Created: 1 Aug 2024 3:32:37pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "../Subsection.h"
#include "../../Knobs/Knob1.h"

class DistortionSubsection : public Subsection {
public:
    DistortionSubsection(int id);

    void paint(juce::Graphics& g) override;
    void resized() override;
    void attachParams(ParameterHandler& parameterHandler) override;
private:
    std::unique_ptr<Knob1> driveKnob;

    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionSubsection)

};