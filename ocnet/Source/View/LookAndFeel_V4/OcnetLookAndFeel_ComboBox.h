/*
  ==============================================================================

    OcnetLookAndFeel_ComboBox.h
    Created: 8 Jul 2024 1:09:47am
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Utils/OcnetTypes.h"

using namespace Ocnet;

class OcnetLookAndFeel_ComboBox : public juce::LookAndFeel_V4
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
        auto cornerSize = 4.0f;

        // Fondo principal
        g.setColour(Palette::Section);
        g.fillRoundedRectangle(0, 0, (float)width, (float)height, cornerSize);

        // Dibujar flecha más grande apuntando hacia abajo
        juce::Path path;
        auto arrowWidth = 12.0f;   // Ajustar ancho de la flecha
        auto arrowHeight = 8.0f;  // Ajustar alto de la flecha
        auto arrowX = buttonX + (buttonW / 2.0f) - (arrowWidth / 2.0f); // Centrar flecha horizontalmente
        auto arrowY = buttonY + (buttonH / 2.0f) - (arrowHeight / 2.0f); // Centrar flecha verticalmente

        path.addTriangle(arrowX, arrowY,                            // Punta izquierda
            arrowX + arrowWidth, arrowY,              // Punta derecha
            arrowX + (arrowWidth / 2.0f), arrowY + arrowHeight); // Punta inferior

        g.setColour(Palette::Text);
        g.fillPath(path);
    }

    juce::Font getComboBoxFont(juce::ComboBox&) override
    {
        return juce::Font("Arial", 15.0f, juce::Font::bold);
    }

    juce::Label* createComboBoxTextBox(juce::ComboBox& box) override
    {
        auto* label = LookAndFeel_V4::createComboBoxTextBox(box);
        label->setColour(juce::Label::textColourId, Palette::Text);
        label->setJustificationType(juce::Justification::centred);
        return label;
    }
};
