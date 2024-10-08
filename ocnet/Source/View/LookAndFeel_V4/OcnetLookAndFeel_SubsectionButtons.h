/*
  ==============================================================================

    OcnetLookAndFeel_2.h
    Created: 23 Aug 2024 1:09:00pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Utils/OcnetTypes.h"

using namespace Ocnet;

class OcnetLookAndFeel_2 : public juce::LookAndFeel_V4
{
public:
    static OcnetLookAndFeel_2& getInstance()
    {
        static OcnetLookAndFeel_2 instance;
        return instance;
    }

    // Sobreescribimos el método para no dibujar el fondo ni el borde del botón
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& /*backgroundColour*/,
        bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/) override
    {
        // No dibujamos nada en el fondo ni en los bordes
    }

    // Sobreescribimos el método para dibujar solo el texto
    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool shouldDrawButtonAsHighlighted, bool /*shouldDrawButtonAsDown*/) override
    {
        juce::Font font("Bahnschrift", 15.0f, juce::Font::bold); // Usar la fuente "Helvetica"
        g.setFont(font);

        juce::Colour textColour = shouldDrawButtonAsHighlighted ? Palette::Accent : Palette::Text;
        shouldDrawButtonAsHighlighted ? button.setMouseCursor(juce::MouseCursor::PointingHandCursor) : button.setMouseCursor(juce::MouseCursor::NormalCursor);
        //shouldDrawButtonAsHighlighted ? button.setSize(button.getBounds().getWidth() + 3, button.getBounds().getHeight() + 3) : button.setSize(button.getBounds().getWidth(), button.getBounds().getHeight());

        g.setColour(textColour);

        // Dibuja el texto centrado en el botón
        g.drawText(button.getButtonText(), button.getLocalBounds(),
            juce::Justification::centred, true);
    }
};