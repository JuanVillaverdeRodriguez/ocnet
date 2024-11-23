/*
  ==============================================================================

    SliderLookAndFeel.h
    Created: 8 Jul 2024 1:09:47am
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Utils/OcnetTypes.h"

using namespace Ocnet;

static class OcnetLookAndFeel : public juce::LookAndFeel_V4
{

public:
    static OcnetLookAndFeel& getInstance()
    {
        static OcnetLookAndFeel instance;
        return instance;
    }

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle, juce::Slider& slider) override
    {
        // Dibuja el recuadro
        g.setColour(juce::Colours::darkgrey);
        g.fillRoundedRectangle(juce::Rectangle<float>(x, y, width, height), 4);


        // Dibuja el valor actual
        g.setColour(Palette::Text);
        g.setFont(24.0f);
        g.drawFittedText(juce::String(static_cast<int>(slider.getValue())), x, y, width, height / 2, juce::Justification::centred, 1);

        // Dibuja el texto "VOICES"
        g.setFont(12.0f);
        g.drawFittedText("VOICES", x, y + height / 2, width, height / 2, juce::Justification::centred, 1);

        g.setColour(juce::Colours::black);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider& slider) override
    {
        // Definir los parámetros básicos
        auto radius = (juce::jmin(width, height) / 2.0f) - 4.0f;
        auto centreX = x + (width * 0.5f);
        auto centreY = y + (height * 0.5f);
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // Dibujar el borde exterior del knob
        //g.setColour(juce::Colours::black);
        //g.drawEllipse(rx, ry, rw, rw, 2.0f); // Grosor del borde

        // Parte central negra
        juce::Path innerCircle;
        innerCircle.addEllipse(rx, ry, radius*2, radius*2);
        g.setColour(Palette::Background);
        g.fillPath(innerCircle);

        // Áreas del arco
        juce::Path backgroundArc;
        backgroundArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, 0.65f);
        g.setColour(Palette::SliderBackground);
        g.fillPath(backgroundArc);


        // Cambiar el color según si el ratón está sobre el slider o no
        juce::Colour fillColour = slider.isMouseOver() ? Palette::SliderFillHovering : Palette::SliderFill;

        // Arco del valor actual
        juce::Path valueArc;
        valueArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, angle, 0.65f);
        g.setColour(fillColour); // Color personalizado del arco
        g.fillPath(valueArc);

        // Parte blanca (sector pequeño)
        juce::Path whiteSection;
        float whiteSectionStartAngle = angle - 0.1f; // Ajusta este valor para que el tamaño y posición sean correctos
        float whiteSectionEndAngle = angle + 0.2f; // Ajusta este valor para el grosor de la sección blanca
        whiteSection.addPieSegment(rx, ry, rw, rw, whiteSectionStartAngle, whiteSectionEndAngle, 0.65f);
        g.setColour(Palette::SliderDot);
        g.fillPath(whiteSection);
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds();

        // Fondo del botón (oscuro)
        g.setColour(Palette::HeaderButtonBackground);
        g.fillRect(bounds);

        // Si el botón está presionado o seleccionado, dibujar la barra indicadora
        if (button.getToggleState())
        {
            g.setColour(Palette::HeaderButtonToggled); // Color de la barra indicadora
            g.fillRect(bounds.removeFromTop(2)); // Barra de 2 píxeles en la parte superior
        }
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        juce::Font font("Bahnschrift", 18.0f, juce::Font::bold); // Usar la fuente "Helvetica"
        g.setFont(font);
        
        juce::Colour textColour = button.getToggleState() ? Palette::Accent : Palette::Text;

        g.setColour(textColour);

        // Centrar el texto en el botón
        g.drawText(button.getButtonText().toUpperCase(), button.getLocalBounds(),
            juce::Justification::centred, true);
    }

};