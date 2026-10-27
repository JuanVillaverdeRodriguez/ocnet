#pragma once

#include "JuceHeader.h"
#include "../../../Controller/GUI_EventHandler.h"
#include "../../../Utils/OcnetTypes.h"

class ModulationLabel : public juce::Label
{
public:
    ModulationLabel()
    {
        setColour(juce::Label::textColourId, Ocnet::Palette::Modulation);
        setFont(Ocnet::Fonts::caps(10.0f, true, 0.05f));
        setJustificationType(juce::Justification::centred);
        setVisible(true);
        setSize(160, 18);
        setInterceptsMouseClicks(false, false);
    }

    void setModulationAmount(float amount)
    {
        setText(parameterModulating.toUpperCase() + "  " + juce::String(amount, 2), juce::dontSendNotification);
    }

    void setParameterModulatingName(const juce::String& parameterModulating) {
        this->parameterModulating = parameterModulating;
    }

    juce::String getParameterModulating() {
        return parameterModulating;
    }

    void paint(juce::Graphics& g) override
    {
        // Pastilla con el nombre del parametro modulado y la cantidad
        juce::GlyphArrangement glyphs;
        glyphs.addLineOfText(getFont(), getText(), 0.0f, 0.0f);
        const float textWidth = glyphs.getBoundingBox(0, -1, true).getWidth();
        auto pill = getLocalBounds().toFloat().withSizeKeepingCentre(textWidth + 16.0f, (float)getHeight());

        g.setColour(Ocnet::Palette::Inset);
        g.fillRoundedRectangle(pill, pill.getHeight() * 0.5f);
        g.setColour(Ocnet::Palette::Modulation.withAlpha(0.5f));
        g.drawRoundedRectangle(pill.reduced(0.5f), pill.getHeight() * 0.5f, 1.0f);

        g.setColour(findColour(juce::Label::textColourId));
        g.setFont(getFont());
        g.drawText(getText(), pill, juce::Justification::centred, false);
    }

private:
    juce::String parameterModulating;
};

class ModulationBubbleListener : public juce::MouseListener, public juce::TextEditor::Listener {
public:
    ModulationBubbleListener(GUI_EventHandler& eventHandler, juce::Slider& listener, juce::String& modulationID);

    ~ModulationBubbleListener() = default;

    void mouseDown(const juce::MouseEvent& event) override;

    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;


private:
    GUI_EventHandler& eventHandler;
    juce::Slider& modBubble;
    juce::String modulationID;
    juce::TextEditor textEditor;

};

class ModulationBubble : public juce::Slider, juce::Slider::Listener
{
public:
    ModulationBubble(juce::String& parameterModulating, GUI_EventHandler& eventHandler);

    void paint(juce::Graphics& g) override;
    void resized() override;
    void moved() override;

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
