#pragma once

#include <JuceHeader.h>
#include "OcnetLookAndFeel.h"

using namespace Ocnet;

// Boton "+ Add ..." de cada seccion: contorno discontinuo a todo lo ancho
class OcnetLookAndFeel_AddButton : public OcnetBaseLookAndFeel
{
public:
    static OcnetLookAndFeel_AddButton& getInstance()
    {
        static OcnetLookAndFeel_AddButton instance;
        return instance;
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
        const juce::Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced(1.0f);
        const auto accent = button.findColour(juce::TextButton::buttonOnColourId);

        if (isMouseOverButton)
        {
            g.setColour(accent.withAlpha(isButtonDown ? 0.14f : 0.08f));
            g.fillRoundedRectangle(bounds, 8.0f);
        }

        juce::Path outline, dashed;
        outline.addRoundedRectangle(bounds, 8.0f);
        const float dashes[] = { 5.0f, 4.0f };
        juce::PathStrokeType(1.0f).createDashedStroke(dashed, outline, dashes, 2);

        g.setColour(isMouseOverButton ? accent.withAlpha(0.8f) : Palette::BorderStrong);
        g.fillPath(dashed);
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool) override
    {
        button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        const auto accent = button.findColour(juce::TextButton::buttonOnColourId);

        g.setColour(isMouseOverButton ? accent.interpolatedWith(Palette::White, 0.4f) : Palette::TextDim);
        g.setFont(Fonts::caps(12.0f, true, 0.1f));
        g.drawText(button.getButtonText().toUpperCase(), button.getLocalBounds(), juce::Justification::centred, false);
    }
};
