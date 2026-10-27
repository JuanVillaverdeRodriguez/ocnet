/*
  ==============================================================================

    OcnetLookAndFeel_2.h
    Created: 23 Aug 2024 1:09:00pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "OcnetLookAndFeel.h"

using namespace Ocnet;

// Botones de la cabecera de las subsecciones (mover, eliminar, bypass) y botones
// de texto planos (selector de sample).
// Los textos "X", "<", ">", "^" y "v" se dibujan como iconos vectoriales.
class OcnetLookAndFeel_2 : public OcnetBaseLookAndFeel
{
public:
    static OcnetLookAndFeel_2& getInstance()
    {
        static OcnetLookAndFeel_2 instance;
        return instance;
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& /*backgroundColour*/,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced(0.5f);

        if (isIconButton(button))
        {
            if (shouldDrawButtonAsHighlighted && button.isEnabled())
            {
                g.setColour(Palette::White.withAlpha(shouldDrawButtonAsDown ? 0.12f : 0.07f));
                g.fillRoundedRectangle(bounds.reduced(2.0f), 4.0f);
            }
            return;
        }

        // Boton de texto: caja hundida
        g.setColour(Palette::Inset);
        g.fillRoundedRectangle(bounds, 5.0f);
        g.setColour(shouldDrawButtonAsHighlighted ? Palette::BorderStrong.brighter(0.2f) : Palette::Border);
        g.drawRoundedRectangle(bounds, 5.0f, 1.0f);
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool shouldDrawButtonAsHighlighted, bool /*shouldDrawButtonAsDown*/) override
    {
        button.setMouseCursor(shouldDrawButtonAsHighlighted ? juce::MouseCursor::PointingHandCursor : juce::MouseCursor::NormalCursor);

        const auto text = button.getButtonText();
        const bool isRemove = text == "X";

        juce::Colour colour = shouldDrawButtonAsHighlighted ? (isRemove ? juce::Colour(0xffff6b6b) : Palette::White)
                                                            : Palette::TextDim;
        if (!button.isEnabled())
            colour = Palette::TextFaint.withAlpha(0.5f);

        g.setColour(colour);

        if (isIconButton(button))
        {
            if (text.isEmpty())
                return;

            auto icon = button.getLocalBounds().toFloat().withSizeKeepingCentre(8.0f, 8.0f);
            juce::Path path;

            if (isRemove)
            {
                path.startNewSubPath(icon.getTopLeft());
                path.lineTo(icon.getBottomRight());
                path.startNewSubPath(icon.getTopRight());
                path.lineTo(icon.getBottomLeft());
            }
            else if (text == "<")
            {
                icon = icon.reduced(1.5f, 0.0f);
                path.startNewSubPath(icon.getRight(), icon.getY());
                path.lineTo(icon.getX(), icon.getCentreY());
                path.lineTo(icon.getRight(), icon.getBottom());
            }
            else if (text == ">")
            {
                icon = icon.reduced(1.5f, 0.0f);
                path.startNewSubPath(icon.getX(), icon.getY());
                path.lineTo(icon.getRight(), icon.getCentreY());
                path.lineTo(icon.getX(), icon.getBottom());
            }
            else if (text == "^")
            {
                icon = icon.reduced(0.0f, 1.5f);
                path.startNewSubPath(icon.getX(), icon.getBottom());
                path.lineTo(icon.getCentreX(), icon.getY());
                path.lineTo(icon.getRight(), icon.getBottom());
            }
            else if (text == "v")
            {
                icon = icon.reduced(0.0f, 1.5f);
                path.startNewSubPath(icon.getX(), icon.getY());
                path.lineTo(icon.getCentreX(), icon.getBottom());
                path.lineTo(icon.getRight(), icon.getY());
            }

            g.strokePath(path, juce::PathStrokeType(1.6f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            return;
        }

        g.setColour(shouldDrawButtonAsHighlighted ? Palette::White : Palette::Text);
        g.setFont(Fonts::ui(13.0f));
        g.drawFittedText(text, button.getLocalBounds().reduced(8, 0), juce::Justification::centred, 1);
    }

private:
    static bool isIconButton(juce::Button& button)
    {
        const auto text = button.getButtonText();
        return text.isEmpty() || text == "X" || text == "<" || text == ">" || text == "^" || text == "v";
    }
};
