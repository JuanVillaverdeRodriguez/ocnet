/*
  ==============================================================================

    OcnetTypes.h
    Created: 9 Aug 2024 1:16:00pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include <juce_core/juce_core.h>

// Cuando se añada algo aqui, añadir tambien la conversion a string en Utils.h
namespace Ocnet {
#define RUN_TESTS 0

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

namespace Ocnet::Palette {
    const juce::Colour Background { 0xff171717 }; // Rich Black
    const juce::Colour BackgroundSecondary { 0xff272727 }; // Raisin Black
    const juce::Colour Primary { 0xff314068 }; // Space Cadet
    const juce::Colour Secondary { 0xff32576C }; // Mexican Pink
    const juce::Colour Accent { 0xff6727C1 }; // Vivid Sky Blue
    const juce::Colour White { 0xffE0E0E0 }; // White Smoke

    //==============================================

    const juce::Colour Text{ White }; // White Smoke

    const juce::Colour Section { Background }; // Fondo de las secciones 
    const juce::Colour Subsection { Primary }; // Fondo de las subsecciones
    const juce::Colour SubsectionTop { BackgroundSecondary };

    const juce::Colour SliderFill { Accent };
    const juce::Colour SliderDot { White };
    const juce::Colour SliderBackground{ BackgroundSecondary };

    const juce::Colour HeaderButtonBackground{ BackgroundSecondary };
    const juce::Colour HeaderButtonToggled{ Accent };

    const juce::Colour BypasButtonOff{ Accent };
    const juce::Colour BypasButtonToggled{ BackgroundSecondary };
}

//namespace Ocnet::Fonts {
//
//}