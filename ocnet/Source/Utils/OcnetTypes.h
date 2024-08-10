/*
  ==============================================================================

    OcnetTypes.h
    Created: 9 Aug 2024 1:16:00pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <juce_core/juce_core.h>

// Cuando se a�ada algo aqui, a�adir tambien la conversion a string en Utils.h
namespace Ocnet {
    enum ProcessorType
    {
        Filter,
        Distortion,
        WavetableOscillator,
        Envelope,
        LFO,
        Unknown
    };
}