/*
  ==============================================================================

    SliderLookAndFeel.h
    Created: 8 Jul 2024 1:09:47am
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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
        g.setColour(juce::Colours::white);
        g.setFont(24.0f);
        g.drawFittedText(juce::String(static_cast<int>(slider.getValue())), x, y, width, height / 2, juce::Justification::centred, 1);

        // Dibuja el texto "VOICES"
        g.setFont(12.0f);
        g.drawFittedText("VOICES", x, y + height / 2, width, height / 2, juce::Justification::centred, 1);

        g.setColour(juce::Colours::black);


    }
};