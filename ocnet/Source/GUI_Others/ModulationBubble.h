#pragma once

#include "JuceHeader.h"
#include "../GUI_EventHandler.h"

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

class ModulationBubbleListener : public juce::MouseListener {
public:
    ModulationBubbleListener(GUI_EventHandler& eventHandler, juce::Slider& listener, juce::String& modulationID);

    ~ModulationBubbleListener() = default;

    void mouseDown(const juce::MouseEvent& event) override;


private:
    GUI_EventHandler& eventHandler;
    juce::Slider& modBubble;
    juce::String modulationID;
};

class ModulationBubble : public juce::Slider, juce::Slider::Listener
{
public:
    ModulationBubble(juce::String& parameterModulating, GUI_EventHandler& eventHandler);

    void paint(juce::Graphics& g) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;

    void setModulationAmount(float amount);

    juce::String getParameterModulating();

    void parentHierarchyChanged() override;

    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;

    juce::String getModulationID();


private:
    float modulationAmount = 0.0f;
    const int initialComponentWidth = 50;
    const int initialComponentHeight = 70;
    const int bubbleSize = 10;
    ModulationLabel modulationLabel;
    ModulationBubbleListener modulationBubbleListener;
    juce::String modulationID;

    juce::Rectangle<int> getBubbleArea() const;

};
