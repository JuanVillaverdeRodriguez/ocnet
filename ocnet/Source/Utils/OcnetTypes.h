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
        Reverb,
        Macro,
        Randomizer,
        Equalizer,
        Delay,
        Unknown
    };
}

namespace Ocnet::Palette {
    // Tonos base (de mas oscuro a mas claro)
    const juce::Colour Background { 0xff0c0e12 };          // Fondo de la ventana
    const juce::Colour Panel { 0xff13161c };               // Paneles principales
    const juce::Colour BackgroundThird { 0xff191c24 };     // Tarjetas (subsecciones)
    const juce::Colour BackgroundSecondary { 0xff20242e }; // Cabeceras y controles
    const juce::Colour Inset { 0xff0b0d11 };               // Graficos, combos y displays
    const juce::Colour Border { 0xff272c37 };
    const juce::Colour BorderStrong { 0xff363c4a };

    // Colores de identidad
    const juce::Colour Primary { 0xff8b7bff };   // Osciladores (violeta)
    const juce::Colour Secondary { 0xff38e1c2 }; // Moduladores y modulaciones (menta)
    const juce::Colour Tertiary { 0xffff9f5a };  // Efectos (ambar)
    const juce::Colour Accent { Primary };
    const juce::Colour AccentBright { 0xffb3a9ff };

    const juce::Colour White { 0xffe9ecf2 };
    const juce::Colour TextDim { 0xff8a92a5 };
    const juce::Colour TextFaint { 0xff5a6172 };

    //==============================================

    const juce::Colour Text{ White };

    const juce::Colour Section { Panel };              // Fondo de las secciones
    const juce::Colour Subsection { BackgroundThird }; // Fondo de las subsecciones
    const juce::Colour SubsectionTop { BackgroundSecondary };

    const juce::Colour SliderFill { Accent };
    const juce::Colour SliderFillHovering { AccentBright };
    const juce::Colour SliderDot { White };
    const juce::Colour SliderBackground{ 0xff282d38 };

    const juce::Colour HeaderButtonBackground{ BackgroundSecondary };
    const juce::Colour HeaderButtonToggled{ Accent };

    const juce::Colour BypasButtonOff{ Accent };
    const juce::Colour BypasButtonToggled{ TextFaint };

    const juce::Colour Modulation { Secondary };

    // Color asociado a cada tipo de subseccion ("Oscillators", "Effects", "Modulators")
    inline juce::Colour forType(const juce::String& type)
    {
        if (type == "Effects")    return Tertiary;
        if (type == "Modulators") return Secondary;
        return Primary;
    }
}

namespace Ocnet::Fonts {
    inline juce::Font ui(float size, bool bold = false)
    {
        return juce::Font(juce::FontOptions("Bahnschrift", size, bold ? juce::Font::bold : juce::Font::plain));
    }

    // Texto en mayusculas con algo de espaciado entre letras (etiquetas, titulos)
    inline juce::Font caps(float size, bool bold = true, float kerning = 0.08f)
    {
        return ui(size, bold).withExtraKerningFactor(kerning);
    }
}

namespace Ocnet::Draw {
    // Fondo hundido con rejilla para los graficos (envolvente, LFO, distorsion...)
    inline void graphBackground(juce::Graphics& g, juce::Rectangle<float> bounds, int columns = 8, int rows = 4)
    {
        const float radius = 6.0f;
        g.setColour(Palette::Inset);
        g.fillRoundedRectangle(bounds, radius);

        g.setColour(Palette::Border.withAlpha(0.5f));
        for (int i = 1; i < columns; ++i)
            g.drawVerticalLine(juce::roundToInt(bounds.getX() + bounds.getWidth() * i / columns), bounds.getY() + 2, bounds.getBottom() - 2);
        for (int i = 1; i < rows; ++i)
            g.drawHorizontalLine(juce::roundToInt(bounds.getY() + bounds.getHeight() * i / rows), bounds.getX() + 2, bounds.getRight() - 2);

        g.setColour(Palette::Border);
        g.drawRoundedRectangle(bounds.reduced(0.5f), radius, 1.0f);
    }

    // Curva con brillo y (opcionalmente) relleno degradado hasta baselineY
    inline void curve(juce::Graphics& g, const juce::Path& path, juce::Rectangle<float> bounds, juce::Colour colour,
                      float baselineY, bool fillBelow = true)
    {
        juce::Graphics::ScopedSaveState state(g);
        juce::Path clip;
        clip.addRoundedRectangle(bounds.reduced(1.0f), 6.0f);
        g.reduceClipRegion(clip);

        if (fillBelow) {
            auto pathBounds = path.getBounds();
            juce::Path fill(path);
            fill.lineTo(pathBounds.getRight(), baselineY);
            fill.lineTo(pathBounds.getX(), baselineY);
            fill.closeSubPath();

            g.setGradientFill(juce::ColourGradient(colour.withAlpha(0.30f), 0.0f, bounds.getY(),
                                                   colour.withAlpha(0.02f), 0.0f, bounds.getBottom(), false));
            g.fillPath(fill);
        }

        g.setColour(colour.withAlpha(0.18f));
        g.strokePath(path, juce::PathStrokeType(6.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        g.setColour(colour);
        g.strokePath(path, juce::PathStrokeType(2.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }
}
