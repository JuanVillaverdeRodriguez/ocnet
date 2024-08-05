/*
  ==============================================================================

    FilterSubsection.h
    Created: 5 Aug 2024 9:45:32pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "EffectsSubsection.h"

class FilterSubsection : public EffectsSubsection {
public:
    FilterSubsection(int id, GUI_EventHandler& eventHandler);
    ~FilterSubsection() override = default;

    void resized() override;

    void attachParams(ParameterHandler& parameterHandler) override;

private:
    std::unique_ptr<Knob1> freqCutKnob;


};