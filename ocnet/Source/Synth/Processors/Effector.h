/*
  ==============================================================================

    Effector.h
    Created: 1 Aug 2024 6:08:28pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Processor.h"

class Effector : public Processor {
public:
    Effector();
    
    virtual float getNextSample(float currentSampleValue) = 0 {};
private:

};