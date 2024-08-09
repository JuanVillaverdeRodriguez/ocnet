/*
  ==============================================================================

    ModulationBubble.cpp
    Created: 6 Aug 2024 1:41:19am
    Author:  TecNo

  ==============================================================================
*/

#include "ModulationBubble.h"
#include "../Utils/Utils.h"

ModulationBubble::ModulationBubble(juce::String& modulationID, GUI_EventHandler& eventHandler) : modulationBubbleListener(eventHandler, *this, modulationID), modulationID(modulationID)
{
    auto [type, id, propertyName] = Utils::splitParameterID(modulationID);

    auto [modTag, parameterModulating] = Utils::splitParameterModulationID(propertyName);

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

    addMouseListener(&modulationBubbleListener, false);
}

void ModulationBubble::sliderValueChanged(juce::Slider* slider)
{
    if (slider == this)
    {
        modulationAmount = static_cast<float>(getValue());
        modulationLabel.setModulationAmount(modulationAmount);
    }
}

void ModulationBubble::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::blue.withAlpha(0.5f));
    g.fillEllipse(getBubbleArea().toFloat());
}

void ModulationBubble::resized()
{
    auto bounds = getLocalBounds();
    auto labelHeight = 15;
    auto bubbleArea = getBubbleArea();

    // Adjust the position of the modulationLabel to ensure it's fully visible
    auto labelX = getX() + bounds.getCentreX() - modulationLabel.getWidth() / 2;
    auto labelY = getY() + bubbleSize + 5;

    modulationLabel.setBounds(labelX, labelY, modulationLabel.getWidth(), labelHeight);
}

juce::String ModulationBubble::getParameterModulating()
{
    return modulationLabel.getParameterModulating();
}

void ModulationBubble::setModulationAmount(float amount)
{
    setValue(amount);
    modulationAmount = amount;
    modulationLabel.setModulationAmount(modulationAmount);
    repaint();
}

void ModulationBubble::parentHierarchyChanged()
{
    if (auto* parent = getParentComponent())
    {
        parent->addChildComponent(modulationLabel);
    }
}


void ModulationBubble::mouseEnter(const juce::MouseEvent& event)
{
    DBG("ModulationBubble::mouseEnter(const juce::MouseEvent& event)");

    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    modulationLabel.setVisible(true);
}

void ModulationBubble::mouseExit(const juce::MouseEvent& event)
{
    DBG("ModulationBubble::mouseExit(const juce::MouseEvent& event)");

    setMouseCursor(juce::MouseCursor::NormalCursor);
    modulationLabel.setVisible(false);
}

juce::String ModulationBubble::getModulationID()
{
    return modulationID;
}

juce::Rectangle<int> ModulationBubble::getBubbleArea() const
{
    return getLocalBounds().withSizeKeepingCentre(bubbleSize, bubbleSize);
}

ModulationBubbleListener::ModulationBubbleListener(GUI_EventHandler& eventHandler, juce::Slider& listener, juce::String& modulationID) : eventHandler(eventHandler), modBubble(listener), modulationID(modulationID)
{

}

void ModulationBubbleListener::mouseDown(const juce::MouseEvent& event)
{
    // Verificamos si el clic fue con el botón derecho del mouse
    if (event.mods.isRightButtonDown())
    {
        juce::PopupMenu menu;
        menu.addItem(1, "Remove modulation");
        menu.addItem(2, "Set value");

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(modBubble),
            [this](int result)
            {
                if (result == 1)
                    eventHandler.onRemoveModulation(modulationID);
                //else if (result == 2)
            });
    }
}
