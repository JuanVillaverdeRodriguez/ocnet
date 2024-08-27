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
        Sampler,
        Unknown
    };
}

namespace Ocnet::Palette {
    const juce::Colour Background { 0xff272727 }; // Rich Black
    const juce::Colour BackgroundSecondary { 0xff353535 }; // Raisin Black
    const juce::Colour BackgroundThird { 0xff525252 }; // Raisin Black
    const juce::Colour Primary { 0xff5B4373 }; // Space Cadet
    const juce::Colour Secondary { 0xff436C73 }; // Mexican Pink
    const juce::Colour Accent { 0xff4B7981 }; // Vivid Sky Blue
    const juce::Colour White { 0xffE3E3E3 }; // White Smoke

    //==============================================

    const juce::Colour Text{ White }; // White Smoke

    const juce::Colour Section { Background }; // Fondo de las secciones 
    const juce::Colour Subsection { BackgroundThird }; // Fondo de las subsecciones
    const juce::Colour SubsectionTop { BackgroundSecondary };

    const juce::Colour SliderFill { Secondary };
    const juce::Colour SliderFillHovering { Accent };
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