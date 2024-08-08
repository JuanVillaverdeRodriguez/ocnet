#pragma once

#include "JuceHeader.h"

class ModulationLabel : public juce::Label
{
public:
    ModulationLabel()
    {
        setColour(juce::Label::textColourId, juce::Colours::white);
        setJustificationType(juce::Justification::centred);
        setVisible(true);
        setSize(300, 15);
        setInterceptsMouseClicks(false, false);
    }

    void setModulationAmount(float amount)
    {
        setText(parameterModulating + juce::String(amount, 2), juce::dontSendNotification);
    }

    void setParameterModulatingName(const juce::String& parameterModulating) {
        this->parameterModulating = parameterModulating;
    }

    juce::String getParameterModulating() {
        return parameterModulating;
    }

private:
    juce::String parameterModulating;
};

class ModulationBubble : public juce::Slider, juce::Slider::Listener
{
public:
    ModulationBubble(const juce::String& parameterModulating)
    {
        setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        setRange(-1.0f, 1.0f, 0.01f);
        setValue(0.0f);
        setAlwaysOnTop(true);
        setSize(initialComponentWidth, initialComponentHeight);

        // Add the modulation label to this component initially
        addAndMakeVisible(modulationLabel);
        modulationLabel.setParameterModulatingName(parameterModulating);

        // Add listener to detect value changes
        addListener(this);
    }

    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == this)
        {
            modulationAmount = static_cast<float>(getValue());
            modulationLabel.setModulationAmount(modulationAmount);
        }
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::blue.withAlpha(0.5f));
        g.fillEllipse(getBubbleArea().toFloat());
    }

    //float getModulationAmount() const { return modulationAmount; }
    void resized() override
    {
        auto bounds = getLocalBounds();
        auto labelHeight = 15;
        auto bubbleArea = getBubbleArea();

        // Adjust the position of the modulationLabel to ensure it's fully visible
        auto labelX = getX() + bounds.getCentreX() - modulationLabel.getWidth() / 2;
        auto labelY = getY() + bubbleSize + 5;

        modulationLabel.setBounds(labelX, labelY, modulationLabel.getWidth(), labelHeight);
    }

    juce::String getParameterModulating() {
        return modulationLabel.getParameterModulating();
    }

    void setModulationAmount(float amount)
    {
        setValue(amount);
        modulationAmount = amount;
        modulationLabel.setModulationAmount(modulationAmount);
        repaint();
    }

    void parentHierarchyChanged() override
    {
        if (auto* parent = getParentComponent())
        {
            parent->addChildComponent(modulationLabel);
        }
    }

    /*
    void mouseDown(const juce::MouseEvent& event) override
    {
        // Ocultar el cursor al hacer clic en la burbuja
        setMouseCursor(juce::MouseCursor::NoCursor);
        // Almacenar la posición inicial del ratón en coordenadas globales
        initialMousePos = event.getEventRelativeTo(this).getScreenPosition();
    }

    void mouseUp(const juce::MouseEvent& event) override
    {
        // Restaurar el cursor al soltar el clic
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
        // Restablecer el cursor a la posición inicial guardada
        juce::Desktop::getInstance().setMousePosition(initialMousePos);
    }*/

    void mouseEnter(const juce::MouseEvent& event) override
    {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
        modulationLabel.setVisible(true);
        repaint();
    }

    void mouseExit(const juce::MouseEvent& event) override
    {
        setMouseCursor(juce::MouseCursor::NormalCursor);
        modulationLabel.setVisible(false);
        repaint();
    }

private:
    float modulationAmount = 0.0f;
    juce::Point<int> initialMousePos;
    const int initialComponentWidth = 50;
    const int initialComponentHeight = 70;
    const int bubbleSize = 10;
    juce::Label textLabel;
    ModulationLabel modulationLabel;

    juce::Rectangle<int> getBubbleArea() const
    {
        return getLocalBounds().withSizeKeepingCentre(bubbleSize, bubbleSize);
    }
};
