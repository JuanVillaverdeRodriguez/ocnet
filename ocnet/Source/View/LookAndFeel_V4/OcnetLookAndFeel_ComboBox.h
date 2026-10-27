/*
  ==============================================================================

    OcnetLookAndFeel_ComboBox.h
    Created: 8 Jul 2024 1:09:47am
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "OcnetLookAndFeel.h"

using namespace Ocnet;

class OcnetLookAndFeel_ComboBox : public OcnetBaseLookAndFeel
{
public:
    static OcnetLookAndFeel_ComboBox& getInstance()
    {
        static OcnetLookAndFeel_ComboBox instance;
        return instance;
    }

    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
        int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& comboBox) override
    {
        const float cornerSize = 6.0f;
        auto bounds = juce::Rectangle<float>(0.0f, 0.0f, (float)width, (float)height).reduced(0.5f);
        const bool hovering = comboBox.isMouseOver(true) || comboBox.isPopupActive();

        g.setColour(Palette::Inset);
        g.fillRoundedRectangle(bounds, cornerSize);
        g.setColour(hovering ? Palette::BorderStrong.brighter(0.25f) : Palette::Border);
        g.drawRoundedRectangle(bounds, cornerSize, 1.0f);

        // Chevron hacia abajo
        auto arrow = juce::Rectangle<float>((float)width - 22.0f, 0.0f, 12.0f, (float)height)
                         .withSizeKeepingCentre(9.0f, 5.0f);
        juce::Path path;
        path.startNewSubPath(arrow.getX(), arrow.getY());
        path.lineTo(arrow.getCentreX(), arrow.getBottom());
        path.lineTo(arrow.getRight(), arrow.getY());

        g.setColour(hovering ? Palette::Text : Palette::TextDim);
        g.strokePath(path, juce::PathStrokeType(1.6f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }

    juce::Font getComboBoxFont(juce::ComboBox&) override
    {
        return Fonts::ui(14.0f, true);
    }

    void positionComboBoxText(juce::ComboBox& box, juce::Label& label) override
    {
        label.setBounds(4, 1, box.getWidth() - 28, box.getHeight() - 2);
        label.setFont(getComboBoxFont(box));
    }

    juce::Label* createComboBoxTextBox(juce::ComboBox& box) override
    {
        auto* label = LookAndFeel_V4::createComboBoxTextBox(box);
        label->setColour(juce::Label::textColourId, Palette::Text);
        label->setJustificationType(juce::Justification::centredLeft);
        label->setMinimumHorizontalScale(0.8f);
        return label;
    }
};
